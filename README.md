# C++ Electronic Exchange Matching Engine  

## Simulates how a stock exchange processes orders  

## Design  

### Include  

* Order.h: properties of an order  
    * id, side (buy/sell), price, quantity, timestamp  
* Trade.h: properties of a completed trade    
    * buy order id, sell order id, price, quantity
* OrderBook.h: properties of the order book  
    * Add order, cancel order, get best bid, get best ask  


Order:
    Represents a limit order.

Trade:
    Represents an execution.

OrderBook:
    Maintains active buy and sell orders.

MatchingEngine:
    Applies matching rules and generates trades.