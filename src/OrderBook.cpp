#include "OrderBook.h"
#include <iostream>
#include <utility>
#include <algorithm>

// Checks whether an incoming order
// can match with opposite side
bool OrderBook::hasMatch(const Order& ord) {
    if (ord.getSide() == Side::Buy) {
        double ask = getBestAsk();
        if (ask == -1) {
            return false;
        }
        return ord.getPrice() >= ask;
    }
    else {
        double bid = getBestBid();
        if (bid == -1) {
            return false;
        }
        return ord.getPrice() <= bid;
    }
}

// Removes orders whose quantity has reached 0
void OrderBook::removeFilledOrders() {
    if (!buyOrders.empty()) {
        auto it = std::prev(buyOrders.end());
        std::queue<Order*>& orders = it->second;

        if (orders.front()->getQuantity() <= 0) {
            orderLookup.erase(orders.front()->getId());
            orders.pop();

            if (orders.empty()) {
                buyOrders.erase(it);
            }
        }
    }

    if (!sellOrders.empty()) {
        auto it = sellOrders.begin();
        std::queue<Order*>& orders = it->second;

        if (orders.front()->getQuantity() <= 0) {
            orderLookup.erase(orders.front()->getId());
            orders.pop();

            if (orders.empty()) {
                sellOrders.erase(it);
            }
        }
    }
}

// Checks whether the book has no active orders
bool OrderBook::isEmpty() const {
    return orderLookup.size() == 0;
}

void OrderBook::reserve(std::size_t capacity) {
    orderLookup.max_load_factor(0.7f);
    orderLookup.reserve(capacity);
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

bool OrderBook::cancelOrder(int id) {
    auto it = orderLookup.find(id);
    if (it == orderLookup.end()) {
        return false;
    }
    Order* ord = it->second;
    if (ord->getSide() == Side::Buy) {
        auto priceIt = buyOrders.find(ord->getPrice());

        if (priceIt == buyOrders.end()) {
            return false;
        }

        std::queue<Order*>& orders = priceIt->second;
        std::queue<Order*> remaining;
        
        while (!orders.empty()) {
            Order* current = orders.front();
            if (current->getId() != id) {
                remaining.push(current);
            }
            orders.pop();
        }

        orders = std::move(remaining);
        if (orders.empty()) {
            buyOrders.erase(priceIt);
        }
    }
    else {
        auto priceIt = sellOrders.find(ord->getPrice());

        if (priceIt == sellOrders.end()) {
            return false;
        }

        std::queue<Order*>& orders = priceIt->second;
        std::queue<Order*> remaining;
        
        while (!orders.empty()) {
            Order* current = orders.front();
            if (current->getId() != id) {
                remaining.push(current);
            }
            orders.pop();
        }

        orders = std::move(remaining);
        if (orders.empty()) {
            sellOrders.erase(priceIt);
        }
    }

    orderLookup.erase(it);
    return true;
}

void OrderBook::matchOrder(Order* ord, std::vector<Trade>& trades) {
    while (ord->getQuantity() > 0 && hasMatch(*ord)) {
        Order* restingOrder = nullptr;
        double executionPrice = 0.0;

        if (ord->getSide() == Side::Buy) {
            auto bestAskLevel = sellOrders.begin();

            restingOrder = bestAskLevel->second.front();
            executionPrice = restingOrder->getPrice();
        }
        else {
            auto bestBidLevel = std::prev(buyOrders.end());

            restingOrder = bestBidLevel->second.front();
            executionPrice = restingOrder->getPrice();
        }

        double matchedQuantity = std::min(
            ord->getQuantity(),
            restingOrder->getQuantity()
        );

        ord->reduceQuantity(matchedQuantity);
        restingOrder->reduceQuantity(matchedQuantity);

        if (ord->getSide() == Side::Buy) {
            trades.emplace_back(
                ord->getId(),
                restingOrder->getId(),
                executionPrice,
                matchedQuantity
            );
        }
        else {
            trades.emplace_back(
                restingOrder->getId(),
                ord->getId(),
                executionPrice,
                matchedQuantity
            );
        }

        removeFilledOrders();
    }

    if (ord->getQuantity() > 0) {
        addOrder(ord);
    }
}

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

void OrderBook::printBook() const {
    printBids();
    std::cout << "\n";
    printAsks();
}
void OrderBook::printBids() const {
    std::cout << "BUYS\n\n";
    for (auto it = buyOrders.rbegin(); it != buyOrders.rend(); ++it)
    {
        std::cout << "Price: " << it->first << "\n";
        auto q = it->second;
        while (!q.empty()) {
            Order* ord = q.front();

            std::cout << "  ID = " << ord->getId() << " Qty = " << ord->getQuantity() << "\n";
            q.pop();
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

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
        std::cout << "\n";
    }
    std::cout << "\n";
}

Order* OrderBook::getOrderById(int id) {
    if (orderLookup.find(id) != orderLookup.end()) {
        return orderLookup[id];
    }
    return nullptr;
}   
