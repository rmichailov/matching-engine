#include "MatchingEngine.h"

void MatchingEngine::reserve(std::size_t capacity) {
    orderBook.reserve(capacity);
}

void MatchingEngine::submitOrder(Order* ord, std::vector<Trade>& trades) {
    orderBook.matchOrder(ord, trades);
}

bool MatchingEngine::cancelOrder(int id) {
    return orderBook.cancelOrder(id);
}

const OrderBook& MatchingEngine::getOrderBook() const {
    return orderBook;
}