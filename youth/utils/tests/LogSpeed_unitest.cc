#include <youth/core/ElapsedTimer.h>
#include <youth/utils/Logging.h>

#include <benchmark/benchmark.h>

#include <unistd.h>

using namespace youth::core;
using namespace youth::utils;

void test(int num)
{
    for (int i = 0; i < num; i++) {
        LOG_INFO << "1234567890qwertyuiopasdfghjklzxcvbnm" << i;
    }
}

static void benchmark_test(benchmark::State &state)
{
    // Perform setup here
    for (auto _ : state) {
        // This code gets timed
        test(state.range(0));
    }
}

// Register the function as a benchmark
BENCHMARK(benchmark_test)->Arg(1000 * 10)->Arg(1000 * 100)->Arg(1000 * 1000);

int main(int argc, char **argv)
{
    Logging::setFileBaseName(argv[0]);
    Logging::setOutputMode(LOGGER_MODE_LOGFILE);
    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv))
        return 1;
    ::benchmark::RunSpecifiedBenchmarks();
    ::benchmark::Shutdown();
    return 0;
}
