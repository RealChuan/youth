/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   YTime.h
 * Author: root
 *
 * Created on 2019年8月4日, 上午7:14
 */

#ifndef YTIME_H
#define YTIME_H

#include <string>

namespace youth
{
class YTime
{
public:
	YTime();
	virtual ~YTime();

	std::string getLogDay();
	std::string getLogTime();
	static int32_t getAbsTimespec(struct timespec *ts, int32_t millisecond);

private:

	void getTime();
	std::string logDay;
	std::string logTime;

};
}

#endif /* YTIME_H */
