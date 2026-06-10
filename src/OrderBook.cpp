#include "OrderBook.h"
#include <iostream>
    // Checks whether an incoming order
    // can match with opposite side
bool hasMatch(const Order& ord); 

// Removes orders whose quantity has reached 0
void removeFilledOrders();

// Checks whether the book has no active orders
bool OrderBook::isEmpty() const {
    return orderLookup.size() == 0;
}

void OrderBook::addOrder(Order* ord) {
    orderLookup[ord->getId()] = ord;

    if (ord->getSide() == Side::Buy) {
        buyOrders[ord->getPrice()].push(ord);
    }
    else {
        sellOrders[ord->getPrice()].push(ord);
    }
}

void cancelOrder(int id);
void matchOrder(Order* ord);

double OrderBook::getBestBid() const {
    if (buyOrders.empty()) {
        return -1;
    } 
    return std::prev(buyOrders.end())->first;
}

double OrderBook::getBestAsk() const {
    if (sellOrders.empty()) {
        return -1;
    }
    return sellOrders.begin()->first;
}

void OrderBook::printBook() const {
    for (const auto& entry : buyOrders) {

    }
}
void OrderBook::printBids() const {
    for (const auto& entry : buyOrders) {
        
    }
}

/*
SELLS

Price: 100.25
    ID=7 Qty=20
    ID=9 Qty=30

Price: 100.50
    ID=11 Qty=10

--------------------------------------------

BUYS

Price: 100.00
    ID=3 Qty=15
    ID=5 Qty=25

Price: 99.75
    ID=8 Qty=20*/
void OrderBook::printAsks() const {
    std::cout << "SELLS\n\n";
    for (const auto& entry : sellOrders) {
        std::cout << "Price: " << entry.first << "\n";
        auto q = entry.second;
        while (!q.empty()) {
            Order* ord = q.front();

            std::cout << "  ID = " << ord->getId() << " Qty = " << ord->getQuantity() << "\n";
            q.pop();
        }
    }
    std::cout << std::endl;
}
Order* getOrderByID(int id);        
