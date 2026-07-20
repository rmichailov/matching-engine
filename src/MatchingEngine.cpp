#include "MatchingEngine.h"

std::vector<Trade> MatchingEngine::submitOrder(Order* ord) {
    return orderBook.matchOrder(ord);
}

bool MatchingEngine::cancelOrder(int id) {
    return orderBook.cancelOrder(id);
}

const OrderBook& MatchingEngine::getOrderBook() const {
    return orderBook;
}