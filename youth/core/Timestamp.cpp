#include "Timestamp.h"

#include <string.h>
#include <sys/time.h>

namespace youth
{

namespace core
{

//static const char *arrWeek[] = {"Sunday", "Monday", "Tuesday",
//                                "Wednessday", "Thursday", "Friday", "Saturday"};

Timestamp Timestamp::currentTimestamp()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t seconds = tv.tv_sec;
    return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}

std::string Timestamp::getDayToString()
{
    getTime();
    char buf[32] = {0};
    snprintf(buf, sizeof(buf), "%4d-%02d-%02d",
             m_tmTime.tm_year + 1900, m_tmTime.tm_mon + 1, m_tmTime.tm_mday);
    return buf;
}

std::string Timestamp::getSecondToString()
{
    getTime();
    char buf[64] = {0};
    snprintf(buf, sizeof(buf), "%4d-%02d-%02d %02d:%02d:%02d",
             m_tmTime.tm_year + 1900, m_tmTime.tm_mon + 1, m_tmTime.tm_mday,
             m_tmTime.tm_hour + 8, m_tmTime.tm_min, m_tmTime.tm_sec);
    return buf;
}

std::string Timestamp::microSecondsToString()
{
    getTime();
    char buf[64] = {0};
    int microseconds = static_cast<int>(m_mcroSecondsSinceEpoch % kMicroSecondsPerSecond);
    snprintf(buf, sizeof buf, "%4d-%02d-%02d %02d:%02d:%02d.%06d",
             m_tmTime.tm_year + 1900, m_tmTime.tm_mon + 1, m_tmTime.tm_mday,
             m_tmTime.tm_hour + 8, m_tmTime.tm_min, m_tmTime.tm_sec,
             microseconds);
    return buf;
}

bool Timestamp::getTime()
{
    time_t seconds = static_cast<time_t>(m_mcroSecondsSinceEpoch / kMicroSecondsPerSecond);
    if(seconds == m_lastSecond)
        return false;   //只格式化微秒
    m_lastSecond = seconds;
    gmtime_r(&seconds, &m_tmTime);
    return true;
}

int32_t Timestamp::getAbsTimespec(struct timespec *ts, int32_t millisecond)
{
    if (nullptr == ts)
        return EINVAL;

    struct timeval tv;
    int32_t ret = gettimeofday(&tv, nullptr);
    if (0 != ret)
        return ret;

    ts->tv_sec = tv.tv_sec;
    ts->tv_nsec = long(static_cast<unsigned long>(tv.tv_usec) * 1000UL);

    ts->tv_sec += static_cast<unsigned long>(millisecond) / 1000UL;
    ts->tv_nsec += static_cast<unsigned long>(millisecond) % 1000UL * 1000000UL;

    ts->tv_sec += static_cast<unsigned long>(ts->tv_nsec) / (1000UL * 1000UL * 1000UL);
    ts->tv_nsec %= (1000UL * 1000UL * 1000UL);

    return 0;
}

}

}
