// This file contains the implementation of class Order present in order.hpp

#include<iostream>
#include<vector>
#include<chrono>
#include<stdlib.h>
#include<map>
#include<thread>
#include<iomanip>
#include<memory>

#include "order.hpp"
#include "helpers.hpp"

Order::Order(int quantity_, double price_, BookSide side_):timestamp(unix_time()), quantity(quantity_), price(price_), side(side_) {}

void Order::set_quantity(int32_t new_qty)
{
	quantity=new_qty; 
}

int32_t Order::get_quantity()
{
	return quantity;
}

double Order::get_price()
{
	return price;
}

time_t Order::get_timestamp()
{
	return timestamp; 
}
