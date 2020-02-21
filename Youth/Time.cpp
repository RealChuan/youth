/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   YTime.cpp
 * Author: root
 *
 * Created on 2019年8月4日, 上午7:14
 */

#include "Time.h"
#include "string.h"
#include <sys/time.h>

using namespace youth;

static const int kMicroSecondsPerSecond = 1000 * 1000;

//static const char *arrWeek[] = {"Sunday", "Monday", "Tuesday",
//                                "Wednessday", "Thursday", "Friday", "Saturday"};

Time::Time()
    :microSecondsSinceEpoch_(0)
{
    now();
}

Time::Time(int64_t ms)
    :microSecondsSinceEpoch_(ms)
{
}

void Time::now()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t seconds = tv.tv_sec;
    microSecondsSinceEpoch_ = seconds * kMicroSecondsPerSecond + tv.tv_usec;
}

//Time Time::now()
//{
//    struct timeval tv;
//    gettimeofday(&tv, NULL);
//    int64_t seconds = tv.tv_sec;
//    return Time(seconds * kMicroSecondsPerSecond + tv.tv_usec);
//}

std::string Time::getDayToString()
{
    getTime();
    char buf[32] = {0};
    snprintf(buf, sizeof(buf), "%4d-%02d-%02d",
             tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday);
    return buf;
}

std::string Time::getSecondToString()
{
    getTime();
    char buf[64] = {0};
    snprintf(buf, sizeof(buf), "%4d-%02d-%02d %02d:%02d:%02d",
             tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
             tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
    return buf;
}

std::string Time::getMSToString()
{
    getTime();
    char buf[64] = {0};
    int microseconds = static_cast<int>(microSecondsSinceEpoch_ % kMicroSecondsPerSecond);
    snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
             tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
             tm_time.tm_hour + 8, tm_time.tm_min, tm_time.tm_sec,
             microseconds);
    return buf;
}

void Time::getTime()
{
    time_t seconds = static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
    gmtime_r(&seconds, &tm_time);
}

int32_t Time::getAbsTimespec(struct timespec *ts, int32_t millisecond)
{
    if (nullptr == ts)
        return EINVAL;

    struct timeval tv;
    int32_t ret = gettimeofday(&tv, nullptr);
    if (0 != ret)
        return ret;

    ts->tv_sec = tv.tv_sec;
    ts->tv_nsec = static_cast<long>(static_cast<unsigned long>(tv.tv_usec) * 1000UL);

    ts->tv_sec += static_cast<unsigned long>(millisecond) / 1000UL;
    ts->tv_nsec += static_cast<unsigned long>(millisecond) % 1000UL * 1000000UL;

    ts->tv_sec += static_cast<unsigned long>(ts->tv_nsec) / (1000UL * 1000UL * 1000UL);
    ts->tv_nsec %= (1000UL * 1000UL * 1000UL);

    return 0;
}
