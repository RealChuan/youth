/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Time.h
 * Author: root
 *
 * Created on 2019年8月4日, 上午7:14
 */

#ifndef TIME_H
#define TIME_H

#include <string>

namespace youth
{
class Time
{
public:
	Time();
	virtual ~Time();

	std::string getLogDay();
	std::string getLogTime();
	static int32_t getAbsTimespec(struct timespec *ts, int32_t millisecond);

private:

	void getTime();
	std::string logDay;
	std::string logTime;

};
}

#endif /* TIME_H */
