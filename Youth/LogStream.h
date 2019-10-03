/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LogStream.h
 * Author: root
 *
 * Created on 2019年8月4日, 上午6:43
 */

#ifndef LOGSTREAM_H
#define LOGSTREAM_H

#include <string>
#include "LogBuffer.h"

namespace youth
{

class LogStream
{
public:
	LogStream();
	virtual ~LogStream();

	const char *GetStreamBuff();
	int GetStreamBuffLen();

	LogStream &operator<<(char);
	LogStream &operator<<(unsigned char);
	LogStream &operator<<(short);
	LogStream &operator<<(unsigned short);
	LogStream &operator<<(int);
	LogStream &operator<<(unsigned int);
	LogStream &operator<<(long);
	LogStream &operator<<(unsigned long);
	LogStream &operator<<(long long);
	LogStream &operator<<(unsigned long long);
	LogStream &operator<<(float);
	LogStream &operator<<(double);
	LogStream &operator<<(char *pChar);
	LogStream &operator<<(const char *pChar);
	LogStream &operator<<(unsigned char *pChar);
	LogStream &operator<<(const unsigned char *pChar);
	LogStream &operator<<(std::string strString);

private:
	typedef LogBuffer<normalBuffer> buffer;
	buffer logBuffer; //这个就是具体的Buffer

};
}

#endif /* LOGSTREAM_H */
