#include "MatchingEngine.h"

#include <iostream>

int main() {
    MatchingEngine engine;

    Order sell(1, Side::Sell, 100.0, 10.0, 1);
    Order buy(2, Side::Buy, 101.0, 6.0, 2);

    engine.submitOrder(&sell);

    std::vector<Trade> trades = engine.submitOrder(&buy);

    for (const Trade& trade : trades) {
        std::cout
            << "Trade: Buy ID " << trade.getBuyId()
            << ", Sell ID " << trade.getSellId()
            << ", Price " << trade.getPrice()
            << ", Quantity " << trade.getQuantity()
            << '\n';
    }

    engine.getOrderBook().printBook();

    return 0;
}