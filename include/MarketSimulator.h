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

    double averageLatencyMicroseconds;
    double medianLatencyMicroseconds;
    double p95LatencyMicroseconds;
    double p99LatencyMicroseconds;
    double maximumLatencyMicroseconds;
};

class MarketSimulator {
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

        double calculatePercentile(std::vector<double>& latencies, double percentile);

    public:
        MarketSimulator(unsigned int seed, double basePrice, double priceRange, double minimumQuantity, double maximumQuantity);

        void generateOrders(std::size_t numberOfOrders);

        SimulationResults runBenchmark();

        const MatchingEngine& getEngine() const;
};