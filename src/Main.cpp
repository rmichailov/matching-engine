#include "MarketSimulator.h"

#include <iomanip>
#include <iostream>

int main()
{
    const std::size_t numberOfOrders = 1'000'000;

    MarketSimulator simulator(
        42,
        100.00,
        2.00,
        1.00,
        100.00
    );

    std::cout << "Generating orders...\n";

    simulator.generateOrders(numberOfOrders);

    std::cout << "Running benchmark...\n";

    SimulationResults results = simulator.runBenchmark();

    std::cout << std::fixed << std::setprecision(3);

    std::cout << "\nBenchmark Results\n";
    std::cout << "-----------------\n";

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

    std::cout << "\nLatency\n";
    std::cout << "-------\n";

    std::cout << "Average: "
              << results.averageLatencyMicroseconds
              << " microseconds\n";

    std::cout << "Median:  "
              << results.medianLatencyMicroseconds
              << " microseconds\n";

    std::cout << "P95:     "
              << results.p95LatencyMicroseconds
              << " microseconds\n";

    std::cout << "P99:     "
              << results.p99LatencyMicroseconds
              << " microseconds\n";

    std::cout << "Maximum: "
              << results.maximumLatencyMicroseconds
              << " microseconds\n";

    return 0;
}