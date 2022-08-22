#include <youth/core/Thread.hpp>
#include <youth/utils/Logging.h>
#include <youth/utils/ThreadPool.h>

#include <iostream>
#include <sstream>

using namespace youth::utils;
using namespace youth::core;

std::atomic_int64_t count = 0;

void test(int i)
{
    count.fetch_add(1);
    std::ostringstream oss;
    oss << Thread::currentThreadId();
    LOG_INFO << i << " " << oss.str();
}

int main(int argc, char **argv)
{
    ThreadPool threadPool("TestThreadPool");
    threadPool.setTaskNum(8);
    threadPool.start(2);
    for (int i = 0; i < 100; i++) {
        threadPool.run([=] { test(i); });
    }
    threadPool.waitForAllDone();

    LOG_INFO << count;
    return 0;
}
