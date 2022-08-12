#include "Timestamp.h"

#include <chrono>
#include <string.h>

namespace youth {

namespace core {

// static const char *arrWeek[] = {"Sunday", "Monday", "Tuesday",
//                                 "Wednessday", "Thursday", "Friday",
//                                 "Saturday"};

Timestamp Timestamp::currentTimestamp()
{
    return Timestamp(std::chrono::duration_cast<std::chrono::microseconds>(
                         std::chrono::system_clock::now().time_since_epoch())
                         .count());
}

std::string Timestamp::getDayToString()
{
    getTime();
    char buf[32] = {0};
    snprintf(buf,
             sizeof(buf),
             "%4d-%02d-%02d",
             m_tmTime.tm_year + 1900,
             m_tmTime.tm_mon + 1,
             m_tmTime.tm_mday);
    return buf;
}

std::string Timestamp::getSecondToString()
{
    getTime();
    char buf[64] = {0};
    snprintf(buf,
             sizeof(buf),
             "%4d-%02d-%02d %02d:%02d:%02d",
             m_tmTime.tm_year + 1900,
             m_tmTime.tm_mon + 1,
             m_tmTime.tm_mday,
             m_tmTime.tm_hour + 8,
             m_tmTime.tm_min,
             m_tmTime.tm_sec);
    return buf;
}

std::string Timestamp::microSecondsToString()
{
    getTime();
    char buf[64] = {0};
    int microseconds = static_cast<int>(m_mcroSecondsSinceEpoch % kMicroSecondsPerSecond);
    snprintf(buf,
             sizeof buf,
             "%4d-%02d-%02d %02d:%02d:%02d.%06d",
             m_tmTime.tm_year + 1900,
             m_tmTime.tm_mon + 1,
             m_tmTime.tm_mday,
             m_tmTime.tm_hour + 8,
             m_tmTime.tm_min,
             m_tmTime.tm_sec,
             microseconds);
    return buf;
}

bool Timestamp::getTime()
{
    time_t seconds = static_cast<time_t>(m_mcroSecondsSinceEpoch / kMicroSecondsPerSecond);
    if (seconds == m_lastSecond) {
        return false; //只格式化微秒
    }
    m_lastSecond = seconds;
    gmtime_r(&seconds, &m_tmTime);
    return true;
}

} // namespace core

} // namespace youth
