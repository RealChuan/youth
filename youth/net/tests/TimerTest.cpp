#include <youth/net/EventLoop.h>
#include <youth/utils/Logging.h>

using namespace youth::net;
using namespace youth::utils;
using namespace youth::core;

//打印时间
void printTest(const char *str)
{
    LOG_INFO << str << "Hello world!";
}

void cacelTimer(EventLoop *eventLoop, TimerId timerId)
{
    eventLoop->cancel(timerId);
}

void quit(EventLoop *eventLoop)
{
    eventLoop->quit();
}

int main()
{
    EventLoop loop;
    LOG_INFO << "Start";
    loop.runAfter(1, std::bind(printTest, "1"));
    auto timerId2 = loop.runEvery(2, std::bind(printTest, "2"));
    loop.runAfter(9, std::bind(cacelTimer, &loop, timerId2));
    auto timerId3 = loop.runEvery(3, std::bind(printTest, "3"));
    loop.runAfter(16, std::bind(cacelTimer, &loop, timerId3));
    loop.runAfter(20, std::bind(quit, &loop));
    loop.loop();
    LOG_INFO << "End";
}
