#pragma once

#include "MatchingEngine.h"

#include <cstddef>
#include <memory>
#include <random>
#include <vector>

struct SimulationResults
{
    std::size_t ordersProcessed;
    std::size_t tradesExecuted;
    double runtimeSeconds;
    double ordersPerSecond;
};

class MarketSimulator
{
    private:
        MatchingEngine engine;

        std::vector<std::unique_ptr<Order>> orders;

        std::mt19937 randomGenerator;

        int nextOrderId;
        int nextTimestamp;

        double basePrice;
        double priceRange;
        double minimumQuantity;
        double maximumQuantity;

        Order* generateRandomOrder();

    public:
        MarketSimulator(unsigned int seed, double basePrice, double priceRange, double minimumQuantity, double maximumQuantity);
        
        SimulationResults run(std::size_t numberOfOrders);
        const MatchingEngine& getEngine() const;
};