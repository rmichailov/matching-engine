## What is a matching engine?

A matching engine is the core software of an exchange; it takes orders and matches buyers with sellers according to certain rules

### Trade Format  
> Buy/Sell Shares @ Price  

Matching engine exectures a trade (real exchanges process millions of orders)

Questions: What does it mean to match buyers with sellers?

## Market Orders vs Limit Orders

Limit Order: Trader specifies the worst price they are willing to accept  
> Buy 100 @ $100: buy 100 shares, but never pay more than $100  
> Order may sit in the book waiting  
> Sell 50 @ $105: sell 50 shares, but never accept less than $105

Market Order: A trader wants immediate execution  
> Buy 100 at market: Buy 100 chares from the cheapest available sellers  
> Market orders never rest in the book

Summary: Limit Orders wait until the price reaches the perfect amount, whereas Market Orders happen immediately.


## The Limit Order Book

The order book stores unmatched limit orders

> Bids (Buy Orders)  
> 100 @ 99  
> 50 @ 98  
>  
> Asks (Sell Orders)  
> 75 @ 101  
> 200 @ 102  

Bid: Buy order (someone wants to buy at a certain value)  
Ask: Sell order (someone wants to sell at a certain value)  
Best Bid: Highest buy price  
Best Ask: Lowest sell price  
Spread: Difference between best ask and best bid  

## Price-Time Priority

**Most important rule**  
### Most exchanges use: Better Price First
> For Buys:  
> Buy @ 101  
> Buy @ 100  
> Buy @ 99  
> 101 has priority  
>  
> For Sell:  
> Sell @ 99  
> Sell @ 100  
> Sell @ 101  
> 99 has priority  

#### Earlier Time First  
> If two orders have the same price, the first order executes first.  
> **Fifo** principle  

## How Matching Works  


### Example Book  
> Bids  
> 100 @ 99  
>   
> Asks  
> 100 @ 101  

#### Incoming  
> Buy 50 @ 101  

#### Check  
> buy price >= best ask  
> Yes, 101 >= 101  

### Resulting Book  
> Bids  
> 100 @ 99  
>   
> Asks  
> 50 @ 101  

## Partial Fills  

### Important Reminders  

#### Book  
> Asks  
> 200 @ 100  

#### Incoming  
> Buy 50 @ 100  

#### Resulting Book  
> Asks  
> 150 @ 100  

Remaining amounts continue matching or go to book  

## Order Cancellation  

Traders constantly cancel orders.  

### Example  
> Order #1234  
> Buy 100 @ 100  
>   
> Cancel #123  

Removes from book.  
Real exchanges get more cancellations than executions.  

## Trade Price  










