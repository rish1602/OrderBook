#include<iostream>
#include<vector>
#include<chrono>
#include<stdlib.h>
#include<map>
#include<thread>
#include<iomanip>
#include<memory>
#include<chrono>
#include<thread>
#include<algorithm>

#include "orderbook.hpp"
#include "order.hpp"
#include "helpers.hpp"



void Orderbook::add_order(int32_t qty, double price, BookSide side)
{
	if(side==bid)
		map_bids[price].push_back(std::make_unique<Order>(qty,price,side));
	else
		map_asks[price].push_back(std::make_unique<Order>(qty,price,side)); 
}


Orderbook::Orderbook()
{
        // seed RNG
	srand (time(NULL)); 
	
	//adding some random dummy orders for bids in the Orderbook
	for(int32_t i=0; i<10; ++i)
	{
		int32_t random_qty1= rand() % 100 +1; 
		int32_t random_qty2= rand() % 100 +1; 
		double random_price= 90.0 + (rand() % 1001)/100.0; 

		add_order(random_qty1, random_price, BookSide::bid); //this function call add_order() to add random orders to map_bids
                std::this_thread::sleep_for(std::chrono::milliseconds(2)); //sleep this thread for 2ms so that new timestamp are generated 
								 //& we call add_order() 
		add_order(random_qty2, random_price, BookSide::bid); 		
	}


	//adding some random dummy orders for asks in the Orderbook
	for(int32_t i=0; i<10; ++i)
	{
		int32_t random_qty1= rand() % 100 +1; 
		int32_t random_qty2= rand() % 100 +1; 
		double random_price= 100.0 + (rand() % 1001)/100.0; 

		add_order(random_qty1, random_price, BookSide::ask); //this function call add_order() to add random orders to map_asks
                std::this_thread::sleep_for(std::chrono::milliseconds(2)); //sleep this thread for 2ms so that new timestamp are generated 
								 //& we call add_order() 
		add_order(random_qty2, random_price, BookSide::ask); 		
	}
}


template<typename T> //clean_leg() checks if the vector associated with the maps are empty, then erase that map
void Orderbook::clean_leg( std::map<double, std::vector<std::unique_ptr<Order>>, T>&map)
{
	for(auto it=map.begin(); it!=map.end(); )    //checking for the loop
	{
		if(it->second.empty())      //if the value of map is empty
			it= map.erase(it);  //then erase the map and automatically move to next pointer. This is the way of doing it
		else ++it;                  //else proceed to the next iterator
	}
}


void Orderbook::remove_empty_keys() //this function calls clean_leg() function to remove both map-bids & map_asks with empty values 
{
	clean_leg(map_bids);
	clean_leg(map_asks);
}


std::tuple<int32_t,double> Orderbook::execute_order(OrderType type, int32_t order_quantity, Side side, double price)
{
	int32_t units_transacted=0;
	double total_value=0; 

	//process() traverses through the map from the end to the begin
	//and for every price it has its corresponding vector of unique_ptr, so sort that vector in ascending order based on timestamp
	/*std::tuple<int32_t,double>*/ auto process=[&](/*std::map<double, std::vector<std::unique_ptr<Order>>>*/ auto& map_offers, Side side)
	{
		for(auto rit=map_offers.rbegin(); rit!=map_offers.rend(); ++rit)
		{
			double price_level= rit->first; 
			auto& vec_quotes= rit->second; 
					
			sort(vec_quotes.begin(), vec_quotes.end(), [](const std::unique_ptr<Order>& a, const std::unique_ptr<Order>& b) {
				return a->get_timestamp() < b->get_timestamp();	});



		    //for the LIMIT ORDERS
		    //LIMIT ORDERS are orders which have to bought and sold at specific price. 
		    //If we have to buy LIMIT ORDER @ price x & we r getting it for price < x, in orderbook, so buy it else leave
		    //If we have to sell LIMIT ORDER @ price x & we are getting its sell price > x, in orderbook, so sell it else leave

			bool can_transact= true;
			if(type==limit)
			{
				if(side==buy && price_level>price) //if we want to buy LIMIT_ORDER & it has buying price ie price_level
					can_transact=false;       // from orderbook, if price_level > our budget price so leave it 
				else if(side==sell && price_level<price)  //if we want to sell LIMIT_ORDER 
					can_transact==false;      //price_level > our price so buy else leave it

			}
			

			//if we can transact for LIMIT_ORDERS, lift as many orders as u can from vector of map
			auto it=vec_quotes.begin(); 
			while(it!=vec_quotes.end() && order_quantity>0 && can_transact)
			{
				int32_t quote_qty= (*it)->get_quantity();  //get quantity present in orderbook map's vector
				uint64_t timestamp= (*it)->get_timestamp(); //get timestamp for that orderbook maps's vector quantity

				if(quote_qty>order_quantity) //if there is large quantity present in orderbook more than our need
				{
					units_transacted+= order_quantity;        //our collected LIMIT_ORDER quantity is added
					total_value+= units_transacted*price_level;  //our collected LIMIT_ORDER value is added

					(*it)->set_quantity(quote_qty-order_quantity);  //since we have taken the orders from vector
										       //so update the quantity in the vector
                                         quote_qty-= order_quantity;   //doing it doesnt make any sense but still did it 
					 order_quantity=0; 
					 break; 

				}else            //if in the vector there is less orderquantity present than our required quantity
				{
					units_transacted+= quote_qty;  //then taken that small quantity present in the maps's vector
					total_value+= units_transacted*price_level; 

					order_quantity-= quote_qty; //update our required quantity 
					it= vec_quotes.erase(it); //since we have taken the entire vector's quantity 
								  //now vector's quantity=0, we dont need it. delete that vector's cell
				}
			}
		} 
	
		remove_empty_keys();    //once the LIMIT_ORDER quantity is completely collected and our quantity needs are satisfied
					//we come out of the function, now we call remove_empty_keys()
					//to check if any map has become empty or not. Empty because we might have taken all quantity
					//from its all cells of maps's vector. If thats the case then remove that map
		return std::make_tuple(units_transacted,total_value); 
	};




	//FOR Orderbook::executeorder() STARTS FROM HERE ACTUALLY
	//for MARKET_ORDERS. These are general types of orders where they are bought and sold @ best available price
	if(type==market)
		return (side==sell)? process(map_bids,Side::sell): process(map_asks, Side::buy);     
	else if(type==limit)
	{
	        int32_t units_transacted=0;
	        double total_value=0; 
	        
		if(side==buy)    //now we want to buy the LIMIT_ORDER
		{
			if(best_quote(BookSide::ask) <= price)  //if given price for buying <= our budget price, we happily buy it
			{
				std::tuple<int32_t,double>fill= process(map_asks, Side::buy); //we fill our order
				add_order(order_quantity, price, BookSide::bid);   //add remaining order to book
				return fill; 
			}else
			{
				//we cannot buy as their price is greater than our budget price
				add_order(order_quantity, price, BookSide::bid); 
				return std::make_tuple(units_transacted, total_value); 
			}
		}else
		{	       //now if we want to sell LIMIT_ORDER
			if(best_quote(BookSide::bid)>=price) //for selling if person willing to pay >= our required price so we sell it
			{
				std::tuple<int32_t,double>fill= process(map_bids,Side::sell); //fill order
				add_order(order_quantity, price, BookSide::ask);  //add to orderbook
				return fill;
			}else
			{
				//else we cannot sell
				add_order(order_quantity, price, BookSide::ask); 
				return std::make_tuple(units_transacted, total_value); 
			}
		}
	}else throw std::runtime_error("INTVALID SELECTION!!"); 
	

}

//"bid" is highest price buyer is willing to pay
//"ask" is lowest price in which seller is willing to sell

double Orderbook::best_quote(BookSide side)
{
	if(side==bid)                                 //if side is bid so start from the highest bid as bid always start from highest
		return std::prev(map_bids.end())->first; //std::prev returns previous iterator to current position iterator
	else if (side==ask)
		return std::prev(map_asks.end())->first; //if ask then start from the lowest
	else return 0.0; 

	
}


template<typename T>                //this given function simply prints the map_asks and map_bids and their total quantities; 
void Orderbook::print_leg(std::map<double, std::vector<std::unique_ptr<Order>>, T>&map, BookSide side)
{
	if(side==ask)
	{
		for(auto &it:map)                        //for a given map, calculate sum of its map's vector quantity 
							 //by calling get_quantity(); and then print the sum
		{
			int32_t sum_quantity=0; 

			for(auto &tit: it.second)
				sum_quantity += tit->get_quantity(); 

			
			std::string color="32";
			std::cout<<"  \t\033[1;"<<color<<"m"<<"$"<<std::setw(6)<<std::fixed<<std::setprecision(2)<<it.first<<std::setw(5)<<sum_quantity<<"\033[0m";
			
			
			for(int32_t i=0; i<sum_quantity/10; ++i)
				std::cout<<" █"; 
			
			std::cout<<"\n\n";
		}

	}else if(side==bid)                             //for a given map, calculate sum of its map's vector quantity 
							//by calling get_quantity(); and then print the sum 
	{
		for(auto it=map.rbegin(); it!=map.rend(); ++it)
		{
			int32_t sum_quantity=0;
       
			for(auto &tit: it->second)
				sum_quantity += tit->get_quantity(); 

			
			std::string color="33";
			std::cout<<"  \t\033[1;"<<color<<"m"<<"$"<<std::setw(6)<<std::fixed<<std::setprecision(2)<<it->first<<std::setw(5)<<sum_quantity<<"\033[0m";
			
			
			
			for(int32_t i=0; i<sum_quantity/10; ++i)
				std::cout<<" █"; 
			
			std::cout<<"\n\n";
		}
	}else return ; 

}



void Orderbook::print()
{
	std::cout<<"  ====================== ORDER-BOOK ====================="<<"\n";
        print_leg(map_asks, BookSide::ask); 

	
	double best_ask= best_quote(BookSide::ask); 
	double best_bid= best_quote(BookSide::bid); 
	std::cout<<"  The BID-ASK SPREAD is "<<10000 * (best_ask-best_bid)/best_bid<<"\n";  //print bid-ask spread 
											  //using the formula given 
											  //in std::cout

	print_leg(map_bids, BookSide::bid);
	std::cout<<"  =======================================================\n"; 

}

























































