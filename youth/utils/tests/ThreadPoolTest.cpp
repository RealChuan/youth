#include <youth/utils/Logging.h>
#include <youth/utils/ThreadPool.h>

using namespace youth::utils;
using namespace youth::core;

std::atomic_int64_t count = 0;

void test(int i)
{
    count.fetch_add(1);
    LOG_INFO << i << " " << Thread::threadIdString(Thread::currentThreadId());
}

int main(int argc, char **argv)
{
    auto cpuCores = ThreadPool::cpuCores();
    LOG_INFO << "CPU cores: " << cpuCores;

    ThreadPool threadPool("TestThreadPool");
    threadPool.setTaskNum(8);
    threadPool.start(cpuCores * 2);
    for (int i = 0; i < 1000; i++) {
        threadPool.run([=] { test(i); });
    }
    threadPool.waitForAllDone();

    LOG_INFO << count;
    return 0;
}
