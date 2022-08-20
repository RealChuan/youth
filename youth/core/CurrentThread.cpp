#include "CurrentThread.h"
#include "Process.hpp"

namespace youth {

namespace core {

__thread int CurrentThread::tidCache = 0;
__thread char CurrentThread::tidString_[32];
__thread int CurrentThread::tidStringLength_ = 6;
static_assert(std::is_same<int, pid_t>::value, "pid_t should be int");

void CurrentThread::cacheTid()
{
    if (tidCache == 0) {
        tidCache = Process::getTid();
        tidStringLength_ = snprintf(tidString_, sizeof tidString_, "%5d ", tidCache);
    }
}

} // namespace core

} // namespace youth
