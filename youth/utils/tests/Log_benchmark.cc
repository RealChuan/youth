#include <youth/utils/Logging.h>

#include <benchmark/benchmark.h>

using namespace youth::utils;

void print()
{
    for (int i = 0; i < 1000; i++) {
        LOG_INFO << i << " "
                 << "1234567890qwertyuiopasdfghjklzxcvbnm";
    }
}

static void benchmark_log(benchmark::State &state)
{
    // Perform setup here
    for (auto _ : state) {
        // This code gets timed
        print();
    }
}

// Register the function as a benchmark
BENCHMARK(benchmark_log);
// Run the benchmark
BENCHMARK_MAIN();
