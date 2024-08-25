#include<iostream>
#include<vector>
#include<chrono>
#include<stdlib.h>
#include<thread>
#include<iomanip>
#include<memory>

#include "enums.hpp"
#pragma once   //pragma once means to include the files once 

class Order
{
	private:
		int32_t quantity;
		double price;
		BookSide side;
		uint64_t timestamp;

	public:
		Order(int32_t quantity_, double price_, BookSide side_);
			

		//setter function
		void set_quantity( int32_t new_qty );

		//getter functions
		int32_t get_quantity();
		double get_price();
		time_t get_timestamp(); //function to return time
};


