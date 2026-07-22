#include "MarketSimulator.h"

#include <iomanip>
#include <iostream>

int main()
{
    const std::size_t numberOfOrders = 1'000'000;

    MarketSimulator throughputSimulator(
        42,
        100.00,
        2.00,
        1.00,
        100.00
    );

    std::cout << "Generating throughput orders...\n";

    throughputSimulator.generateOrders(numberOfOrders);

    std::cout << "Running throughput benchmark...\n";

    SimulationResults throughputResults =
        throughputSimulator.runThroughputBenchmark();

    MarketSimulator latencySimulator(
        42,
        100.00,
        2.00,
        1.00,
        100.00
    );

    std::cout << "Generating latency orders...\n";

    latencySimulator.generateOrders(numberOfOrders);

    std::cout << "Running latency benchmark...\n";

    SimulationResults latencyResults =
        latencySimulator.runLatencyBenchmark();

    std::cout << std::fixed << std::setprecision(3);

    std::cout << "\nThroughput Results\n";
    std::cout << "------------------\n";

    std::cout << "Orders processed: "
              << throughputResults.ordersProcessed << '\n';

    std::cout << "Trades executed:  "
              << throughputResults.tradesExecuted << '\n';

    std::cout << "Runtime:          "
              << throughputResults.runtimeSeconds
              << " seconds\n";

    std::cout << "Throughput:       "
              << throughputResults.ordersPerSecond
              << " orders/second\n";

    std::cout << "\nLatency Results\n";
    std::cout << "---------------\n";

    std::cout << "Orders processed: "
              << latencyResults.ordersProcessed << '\n';

    std::cout << "Trades executed:  "
              << latencyResults.tradesExecuted << '\n';

    std::cout << "Average: "
              << latencyResults.averageLatencyMicroseconds
              << " microseconds\n";

    std::cout << "Median:  "
              << latencyResults.medianLatencyMicroseconds
              << " microseconds\n";

    std::cout << "P95:     "
              << latencyResults.p95LatencyMicroseconds
              << " microseconds\n";

    std::cout << "P99:     "
              << latencyResults.p99LatencyMicroseconds
              << " microseconds\n";

    std::cout << "Maximum: "
              << latencyResults.maximumLatencyMicroseconds
              << " microseconds\n";

    return 0;
}