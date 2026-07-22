#pragma once

#include "OrderBook.h"
#include <vector>

class MatchingEngine {
    private:
        OrderBook orderBook;
    
    public:
        void submitOrder(Order* ord, std::vector<Trade>& trades);
        bool cancelOrder(int id);
        const OrderBook& getOrderBook() const;
};