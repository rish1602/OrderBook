#include<iostream>
#include<vector>
#include<chrono>
#include<stdlib.h>
#include<thread>
#include<iomanip>
#include<memory>
#include<map>

#include "order.hpp"
#pragma once

class Orderbook
{
	private:
		std::map<double, std::vector<std::unique_ptr<Order>>, std::less<double>> map_bids; //map for buying orders
		std::map<double, std::vector<std::unique_ptr<Order>>, std::greater<double>> map_asks; //map for selling orders

	public:
		void add_order(int32_t qty, double price, BookSide side); //function to add order to orderbook	
		Orderbook(); 

		template<typename T>
			void clean_leg(std::map<double, std::vector<std::unique_ptr<Order>>, T>&map);

		void remove_empty_keys(); //function to remove empty keys

		std::tuple<int32_t, double>execute_order( OrderType type, int32_t order_quantity, Side side, double price=0); 

		double best_quote(BookSide side); //function to retrieve the best quote from orderbook

		template<typename T>
			void print_leg(std::map<double, std::vector<std::unique_ptr<Order>>, T>&map, BookSide side);

		void print(); 


};

//class Orderbook represents orderbook which is collection of buy and sell orders. 
//it has the following methods-
//add orders(),  execute orders(),  retrieve best quote()
//
//orderbook is implemented as ordered map map_bids (map for buying orders) and map_asks (map for selling orders)
//map_bids is sorted in ascending order of price which is double
//map_asks is sorted in descending order of price which is double
