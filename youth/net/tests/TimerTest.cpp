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
    EventLoop eventLoop;
    LOG_INFO << "Start";
    eventLoop.runAfter(1, std::bind(printTest, "1"));
    TimerId timerId2 = eventLoop.runEvery(2, std::bind(printTest, "2"));
    eventLoop.runAfter(9, std::bind(cacelTimer, &eventLoop, timerId2));
    TimerId timerId3 = eventLoop.runEvery(3, std::bind(printTest, "3"));
    eventLoop.runAfter(16, std::bind(cacelTimer, &eventLoop, timerId3));
    eventLoop.runAfter(20, std::bind(quit, &eventLoop));
    eventLoop.loop();
    LOG_INFO << "End";
}
