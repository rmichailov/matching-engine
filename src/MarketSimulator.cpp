#include "MarketSimulator.h"

#include <algorithm>
#include <chrono>
#include <numeric>
#include <stdexcept>

MarketSimulator::MarketSimulator(unsigned int seed, double basePrice, double priceRange, double minimumQuantity, double maximumQuantity) {
    randomGenerator = std::mt19937(seed);

    nextOrderId = 1;
    nextTimestamp = 1;

    this->basePrice = basePrice;
    this->priceRange = priceRange;
    this->minimumQuantity = minimumQuantity;
    this->maximumQuantity = maximumQuantity;

    if (basePrice <= 0.0) {
        throw std::invalid_argument("Base price must be positive.");
    }

    if (priceRange < 0.0) {
        throw std::invalid_argument("Price range cannot be negative.");
    }

    if (minimumQuantity <= 0.0) {
        throw std::invalid_argument("Minimum quantity must be positive.");
    }

    if (maximumQuantity < minimumQuantity) {
        throw std::invalid_argument("Maximum quantity must be at least the minimum quantity.");
    }
}

Order* MarketSimulator::generateRandomOrder() {
    std::bernoulli_distribution sideDistribution(0.5);

    std::uniform_real_distribution<double> priceDistribution(
        basePrice - priceRange,
        basePrice + priceRange
    );

    std::uniform_real_distribution<double> quantityDistribution(
        minimumQuantity,
        maximumQuantity
    );

    Side side;

    if (sideDistribution(randomGenerator)) {
        side = Side::Buy;
    }
    else {
        side = Side::Sell;
    }

    double price = priceDistribution(randomGenerator);
    double quantity = quantityDistribution(randomGenerator);

    std::unique_ptr<Order> order = std::make_unique<Order>(
        nextOrderId,
        side,
        price,
        quantity,
        nextTimestamp
    );

    nextOrderId++;
    nextTimestamp++;

    Order* orderPointer = order.get();

    orders.push_back(std::move(order));

    return orderPointer;
}

void MarketSimulator::generateOrders(std::size_t numberOfOrders) {
    orders.reserve(orders.size() + numberOfOrders);

    for (std::size_t i = 0; i < numberOfOrders; i++) {
        generateRandomOrder();
    }
}

double MarketSimulator::calculatePercentile(std::vector<double>& latencies, double percentile) {
    if (latencies.empty()) {
        return 0.0;
    }

    std::size_t index = static_cast<std::size_t>(
        percentile * static_cast<double>(latencies.size() - 1)
    );

    return latencies[index];
}

SimulationResults MarketSimulator::runThroughputBenchmark() {
    std::size_t tradeCount = 0;

    auto benchmarkStart = std::chrono::steady_clock::now();

    for (const std::unique_ptr<Order>& order : orders)
    {
        std::vector<Trade> trades = engine.submitOrder(order.get());

        tradeCount += trades.size();
    }

    auto benchmarkEnd = std::chrono::steady_clock::now();

    std::chrono::duration<double> totalDuration =
        benchmarkEnd - benchmarkStart;

    double runtimeSeconds = totalDuration.count();

    double ordersPerSecond = 0.0;

    if (runtimeSeconds > 0.0)
    {
        ordersPerSecond =
            static_cast<double>(orders.size()) / runtimeSeconds;
    }

    return SimulationResults{
        orders.size(),
        tradeCount,
        runtimeSeconds,
        ordersPerSecond,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0
    };
}

SimulationResults MarketSimulator::runLatencyBenchmark()
{
    std::size_t tradeCount = 0;

    std::vector<double> latencies;
    latencies.reserve(orders.size());

    auto benchmarkStart = std::chrono::steady_clock::now();

    for (const std::unique_ptr<Order>& order : orders)
    {
        auto orderStart = std::chrono::steady_clock::now();

        std::vector<Trade> trades = engine.submitOrder(order.get());

        auto orderEnd = std::chrono::steady_clock::now();

        tradeCount += trades.size();

        std::chrono::duration<double, std::micro> orderDuration =
            orderEnd - orderStart;

        latencies.push_back(orderDuration.count());
    }

    auto benchmarkEnd = std::chrono::steady_clock::now();

    std::chrono::duration<double> totalDuration =
        benchmarkEnd - benchmarkStart;

    double runtimeSeconds = totalDuration.count();

    double ordersPerSecond = 0.0;

    if (runtimeSeconds > 0.0)
    {
        ordersPerSecond =
            static_cast<double>(orders.size()) / runtimeSeconds;
    }

    double totalLatency = std::accumulate(
        latencies.begin(),
        latencies.end(),
        0.0
    );

    double averageLatency = 0.0;

    if (!latencies.empty())
    {
        averageLatency =
            totalLatency / static_cast<double>(latencies.size());
    }

    std::sort(latencies.begin(), latencies.end());

    double medianLatency =
        calculatePercentile(latencies, 0.50);

    double p95Latency =
        calculatePercentile(latencies, 0.95);

    double p99Latency =
        calculatePercentile(latencies, 0.99);

    double maximumLatency = 0.0;

    if (!latencies.empty())
    {
        maximumLatency = latencies.back();
    }

    return SimulationResults{
        orders.size(),
        tradeCount,
        runtimeSeconds,
        ordersPerSecond,
        averageLatency,
        medianLatency,
        p95Latency,
        p99Latency,
        maximumLatency
    };
}

const MatchingEngine& MarketSimulator::getEngine() const
{
    return engine;
}