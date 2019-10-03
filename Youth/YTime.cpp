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

#include "YTime.h"
#include "string.h"
#include <sys/time.h>

using namespace youth;

static const char *arrWeek[] = {"Sunday", "Monday", "Tuesday",
								"Wednessday", "Thursday", "Friday", "Saturday"};

YTime::YTime()
{
}

YTime::~YTime()
{
}

std::string YTime::getLogDay()
{
	getTime();
	return logDay;
}

std::string YTime::getLogTime()
{
	getTime();
	return logTime;
}

void YTime::getTime()
{
	char Buf1[32];
	char Buf2[32];
	memset(Buf1, 0, sizeof Buf1);
	memset(Buf2, 0, sizeof Buf2);

	time_t time_t;
	struct tm *_t;
	time(&time_t);
	_t = localtime(&time_t);

	snprintf(Buf1, sizeof Buf1, "%04d-%02d-%02d.%s ",
			 _t->tm_year + 1900, _t->tm_mon + 1, _t->tm_mday, arrWeek[_t->tm_wday]);

	snprintf(Buf2, sizeof Buf2, "%04d-%02d-%02d %02d:%02d:%02d %s ",
			 _t->tm_year + 1900, _t->tm_mon + 1, _t->tm_mday,
			 _t->tm_hour, _t->tm_min, _t->tm_sec, arrWeek[_t->tm_wday]);

	logDay = Buf1;
	logTime = Buf2;

}

int32_t YTime::getAbsTimespec(struct timespec *ts, int32_t millisecond)
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
