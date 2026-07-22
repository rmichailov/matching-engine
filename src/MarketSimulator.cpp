#include "MarketSimulator.h"

#include <chrono>
#include <stdexcept>

MarketSimulator::MarketSimulator(
    unsigned int seed,
    double basePrice,
    double priceRange,
    double minimumQuantity,
    double maximumQuantity
)
    : randomGenerator(seed),
      nextOrderId(1),
      nextTimestamp(1),
      basePrice(basePrice),
      priceRange(priceRange),
      minimumQuantity(minimumQuantity),
      maximumQuantity(maximumQuantity)
{
    if (basePrice <= 0.0)
        throw std::invalid_argument("Base price must be positive.");

    if (priceRange < 0.0)
        throw std::invalid_argument("Price range cannot be negative.");

    if (minimumQuantity <= 0.0)
        throw std::invalid_argument("Minimum quantity must be positive.");

    if (maximumQuantity < minimumQuantity)
        throw std::invalid_argument(
            "Maximum quantity must be at least the minimum quantity."
        );
}

Order* MarketSimulator::generateRandomOrder()
{
    std::bernoulli_distribution sideDistribution(0.5);

    std::uniform_real_distribution<double> priceDistribution(
        basePrice - priceRange,
        basePrice + priceRange
    );

    std::uniform_real_distribution<double> quantityDistribution(
        minimumQuantity,
        maximumQuantity
    );

    Side side = sideDistribution(randomGenerator)
        ? Side::Buy
        : Side::Sell;

    double price = priceDistribution(randomGenerator);
    double quantity = quantityDistribution(randomGenerator);

    auto order = std::make_unique<Order>(
        nextOrderId,
        side,
        price,
        quantity,
        nextTimestamp
    );

    ++nextOrderId;
    ++nextTimestamp;

    Order* orderPointer = order.get();

    orders.push_back(std::move(order));

    return orderPointer;
}

SimulationResults MarketSimulator::run(std::size_t numberOfOrders)
{
    std::size_t tradeCount = 0;

    orders.reserve(orders.size() + numberOfOrders);

    auto startTime = std::chrono::steady_clock::now();

    for (std::size_t i = 0; i < numberOfOrders; ++i)
    {
        Order* order = generateRandomOrder();

        std::vector<Trade> trades = engine.submitOrder(order);

        tradeCount += trades.size();
    }

    auto endTime = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsedTime = endTime - startTime;

    double runtimeSeconds = elapsedTime.count();

    double ordersPerSecond =
        runtimeSeconds > 0.0
            ? static_cast<double>(numberOfOrders) / runtimeSeconds
            : 0.0;

    return SimulationResults{
        numberOfOrders,
        tradeCount,
        runtimeSeconds,
        ordersPerSecond
    };
}

const MatchingEngine& MarketSimulator::getEngine() const
{
    return engine;
}