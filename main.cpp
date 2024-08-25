#include<iostream>
#include<fstream>

#include "order.cpp"
#include "orderbook.cpp"
#include "helpers.hpp"

int main(int32_t argc, char* argv[])
{
	std::fstream f; 
	if(f.is_open())
		std::cout<<f.rdbuf()<<"\n";;

	Orderbook obj; 

	while(true)
	{
		
		std::cout<<"                    OPTIONS \n ---------------------------------------------------------- \n";
		std::cout<<"  1) Print Orderbook    2) Submit Orderbook   3)  Exit\n ----------------------------------------------------------\n"<<"\n";
		int32_t action; std::cout<<"  "; std::cin>>action; 
		std::cout<<"\n";

		if(action==1)
			obj.print();
		else if(action==2)
		{
			int32_t order_type_input, quantity, side_input;
			double price; 

			std::cout<<"  Enter the order type-"<<"\n";
			std::cout<<"  1) Market order       2) Limit order"<<"\n";
			std::cout<<"  "; std::cin>>order_type_input;
			OrderType order_type= static_cast<OrderType>(order_type_input); 

			std::cout<<"  Enter the side \n";
			std::cout<<"  1) Buy       2) Sell"<<"\n";
			std::cout<<"  "; std::cin>>side_input; 
			Side side= static_cast<Side>(side_input); 

			std::cout<<"  Enter the quantity \n";
			std::cout<<"  "; std::cin>>quantity; 

			if(order_type==market)
			{
				std::cout<<"  Submitting market "<<((side==buy)?"buy":"sell")<<" order for "<<quantity<<" units"<<"\n";
				u_int64_t start_time= unix_time(); 
				std::tuple<int32_t,double>fill= obj.execute_order(order_type, quantity, side);
				u_int64_t end_time= unix_time(); 
				
				
				std::cout<<"  Filled "<<std::get<0>(fill)<<"/"<<quantity<<" units at $"<<std::get<1>(fill)/std::get<0>(fill)<<" Average price. Time taken: "<<(end_time-start_time)<<" nano seconds\033[0m"<<"\n";

			}else if(order_type==limit)
			{
				std::cout<<"  Enter price limit: \n";
				std::cout<<"  "; std::cin>>price; 

				std::cout<<"  Submitting limit "<<((side==buy)?"buy":"sell")<<" order for "<<quantity<<" units "<<"and limit price being "<<price<<"\n";

				u_int64_t start_time= unix_time(); 
				std::tuple<int32_t,double>fill= obj.execute_order(order_type, quantity, side, price);
				u_int64_t end_time= unix_time(); 
				
				
				std::cout<<"  Filled "<<std::get<0>(fill)<<"/"<<quantity<<" units at $"<<std::get<1>(fill)/std::get<0>(fill)<<" Average price. Time taken: "<<(end_time-start_time)<<" nano seconds\033[0m" <<"\n";
			}
			std::cout<<"\n";
			

		}else if(action==3)  break;
		else std::cout<<"  Enter the correct option \n\n"; 
		
		

	}

}
