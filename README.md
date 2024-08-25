# OrderBook
This is my c++ implementation of Order-book. It executes two specific orders- Market orders and Limit orders. I've defines two orderbooks- bids orderbook and asks orderbook. These orderbook are implemented as ordered_maps.  
i.e. std::map<double, std::vector<std::unique_ptr<Order>>>.


<img width="397" alt="maps snip" src="https://github.com/user-attachments/assets/109fab86-1a78-4697-9c09-b5acf7b51f70">

                            
Some important **features** are-

        1) The orderbook is pre-populated with random orders.
        2) It handles Market orders and Limit orders.
        3) Displays bar chart visualization
        4) Whole and partial fills

Here there are 7 main files- 

1) **enums.hpp**- which stores general variables like bid, ask, buy, sell, market, limit.

2)  **helpers.hpp**- stores function to generate time

3)  **order.hpp**- defines order class that stores our very important variables- quantity, price, side and timestamp, along with getter and setter functions

4)  **order.cpp**- it defines all getter and setter function members that were declared in order class of order.hpp.

5)  **orderbook.hpp**- defines orderbook class that holds our 2 orderbooks- bids orderbook and asks orderbook.

6)  **orderbook.cpp**- it defines all functions members that were declared in orderbook class of orderbook.hpp i.e defines important functions like- add_order(), remove empty_order from map, execute_order() etc.

7)  **main.cpp**- the starting point of the entire programme.


-----------------------------
 **For running the program just compile and run main.cpp**
-----------------------------

**SOME SCREENSHOTS OF PROGRAM**
-----------------------------


---------------
<img width="357" alt="snip1 - main option" src="https://github.com/user-attachments/assets/336f3ffe-6905-4222-826b-f383bf170161">

---------------
<img width="341" alt="snip2- after_selecting_option2" src="https://github.com/user-attachments/assets/405d5db0-395a-4a66-af20-448091ce6e70">

--------------
<img width="371" alt="snip3 after_selecting_LIMIT_ORDER" src="https://github.com/user-attachments/assets/ad6c48a5-22a7-4d6f-8131-c6405d9888a5">

--------------
<img width="326" alt="snip4 after selecting buy or sell" src="https://github.com/user-attachments/assets/dc7bbee1-4711-441e-b8f6-a959f11a5e6b">

----------------
<img width="421" alt="snip5 after selecting quantity and price" src="https://github.com/user-attachments/assets/6705c579-c240-4407-942e-196d77ae0f34">

----------------
<img width="294" alt="snip orderbook s" src="https://github.com/user-attachments/assets/091fbd9c-6b13-41b2-831e-c0f45d7adc1a">

-------------------
For me to understand-->
![snip for me](https://github.com/user-attachments/assets/32e72a8a-1c66-439c-84ff-4a6a3c2b5d80)



