#pragma once

#include "OrderBook.h"
#include <vector>

class MatchinEngine {
    private:
        OrderBook orderBook;
    
    public:
        std::vector<Trade> submitOrder(Order* ord);
        bool cancelOrder(int id);
        const OrderBook& getOrderBook() const;
}