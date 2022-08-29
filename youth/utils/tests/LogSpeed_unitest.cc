#include <gflags/gflags.h>
#include <youth/core/ElapsedTimer.h>
#include <youth/utils/Logging.h>

#include <benchmark/benchmark.h>
#include <glog/logging.h>

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

void test_g(int num)
{
    for (int i = 0; i < num; i++) {
        LOG(INFO) << "1234567890qwertyuiopasdfghjklzxcvbnm" << i;
    }
}

static void benchmark_test_g(benchmark::State &state)
{
    // Perform setup here
    for (auto _ : state) {
        // This code gets timed
        test_g(state.range(0));
    }
}

// Register the function as a benchmark
BENCHMARK(benchmark_test_g)->Arg(1000 * 10)->Arg(1000 * 100)->Arg(1000 * 1000);

int main(int argc, char **argv)
{
    Logging::setDirectoryAndBaseName("/tmp/Youth/", argv[0]); // must
    Logging::setOutputMode(LOGGER_MODE_LOGFILE);

    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();
    google::SetLogFilenameExtension(".log");
    google::EnableLogCleaner(3);
    //google::DisableLogCleaner();
    //FLAGS_alsologtostderr = true;  //是否将日志输出到文件和stderr
    FLAGS_colorlogtostderr = true; //是否启用不同颜色显示
    fLS::FLAGS_log_dir = "/tmp/Youth/";

    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv))
        return 1;
    ::benchmark::RunSpecifiedBenchmarks();
    ::benchmark::Shutdown();

    google::ShutdownGoogleLogging();
    return 0;
}
