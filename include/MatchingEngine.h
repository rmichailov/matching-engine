#pragma once

#include "OrderBook.h"
#include <vector>
#include <cstddef>

class MatchingEngine {
    private:
        OrderBook orderBook;
    
    public:
        void reserve(std::size_t capacity);
        void submitOrder(Order* ord, std::vector<Trade>& trades);
        bool cancelOrder(int id);
        const OrderBook& getOrderBook() const;
};