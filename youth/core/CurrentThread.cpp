#include "CurrentThread.h"
#include "ProcessMsg.h"
#include "Timestamp.h"

namespace youth {

namespace CurrentThread {

__thread int tidCache = 0;
__thread char tidString_[32];
__thread int tidStringLength_ = 6;
static_assert(std::is_same<int, pid_t>::value, "pid_t should be int");

void cacheTid()
{
    if (tidCache == 0){
        tidCache = ProcessMsg::getTid();
        tidStringLength_ = snprintf(tidString_, sizeof tidString_, "%5d ", tidCache);
    }
}

void sleepUsec(int64_t usec)
{
    struct timespec ts = { 0, 0 };
    ts.tv_sec = static_cast<time_t>(usec / Timestamp::kMicroSecondsPerSecond);
    ts.tv_nsec = static_cast<long>(usec % Timestamp::kMicroSecondsPerSecond * 1000);
    ::nanosleep(&ts, NULL);
}

bool isMainThread()
{
    return tid() == ::getpid();
}

}

}
