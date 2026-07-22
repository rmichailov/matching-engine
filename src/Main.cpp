#include "MarketSimulator.h"

#include <iomanip>
#include <iostream>

int main()
{
    const std::size_t numberOfOrders = 1'000'000;

    MarketSimulator simulator(
        42,       // Random seed
        100.00,   // Base price
        2.00,     // Prices range from 98 to 102
        1.00,     // Minimum quantity
        100.00    // Maximum quantity
    );

    SimulationResults results = simulator.run(numberOfOrders);

    std::cout << std::fixed << std::setprecision(3);

    std::cout << "\nSimulation Results\n";
    std::cout << "------------------\n";
    std::cout << "Orders processed: "
              << results.ordersProcessed << '\n';

    std::cout << "Trades executed:  "
              << results.tradesExecuted << '\n';

    std::cout << "Runtime:          "
              << results.runtimeSeconds
              << " seconds\n";

    std::cout << "Throughput:       "
              << results.ordersPerSecond
              << " orders/second\n";

    return 0;
}