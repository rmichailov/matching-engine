#pragma once
#include "Order.h"
#include "Trade.h"
#include <unordered_map>
#include <map>
#include <queue>

class OrderBook {
    private:
        std::unordered_map<int, Order*> orderLookup;
        std::map<double, std::queue<Order*>> buyOrders;
        std::map<double, std::queue<Order*>> sellOrders;

        // Helper methods

        // Checks whether an incoming order
        // can match with opposite side
        bool hasMatch(const Order& ord); 

        // Removes orders whose quantity has reached 0
        void removeFilledOrders();

        // Checks whether the book has no active orders
        bool isEmpty() const;

    public:
        void addOrder(Order* ord);
        bool cancelOrder(int id);
        std::vector<Trade> matchOrder(Order* ord);
        double getBestBid() const;
        double getBestAsk() const;

        void printBook() const;
        void printBids() const;
        void printAsks() const;
        Order* getOrderById(int id);        

};