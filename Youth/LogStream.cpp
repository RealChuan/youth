/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   LogStream.cpp
 * Author: root
 *
 * Created on 2019年8月4日, 上午6:43
 */

#include "LogStream.h"

using namespace youth;

//下面这个函数模板是用来实现整型转换为字符串

const char digits[] = "9876543210123456789";
const static char* zero = digits + 9;
static_assert(sizeof (digits) == 20, "wrong number of digits");

const char digitsHex[] = "0123456789ABCDEF";
static_assert(sizeof digitsHex == 17, "wrong number of digitsHex");

// Efficient Integer to String Conversions, by Matthew Wilson.

template<typename T>
size_t convert(char buf[], T value)
{
	T i = value;
	char* p = buf;

	do
	{
		int lsd = static_cast<int> (i % 10);
		i /= 10;
		*p++ = zero[lsd];
	}
	while (i != 0);

	if (value < 0)
	{
		*p++ = '-';
	}
	*p = '\0';
	std::reverse(buf, p);

	return static_cast<size_t>(p - buf);
}

size_t convertHex(char buf[], uintptr_t value)
{
	uintptr_t i = value;
	char* p = buf;

	do
	{
		int lsd = static_cast<int> (i % 16);
		i /= 16;
		*p++ = digitsHex[lsd];
	}
	while (i != 0);

	*p = '\0';
	std::reverse(buf, p);

	return static_cast<size_t>(p - buf);
}

LogStream::LogStream()
{
}

LogStream::~LogStream()
{
}

const int iFloatMaxStrLen = 16;
const int iDoubleMaxStrLen = 32;

const char *LogStream::GetStreamBuff()
{
	return logBuffer.data();
}

int LogStream::GetStreamBuffLen()
{
	return logBuffer.length();
}

LogStream &LogStream::operator<<(char value)
{
	int Len = static_cast<int>(convert(logBuffer.current(), value));
	logBuffer.appendLen(Len);
	return *this;
}

LogStream &LogStream::operator<<(unsigned char value)
{
	int Len = static_cast<int>(convert(logBuffer.current(), value));
	logBuffer.appendLen(Len);
	return *this;
}

LogStream &LogStream::operator<<(short value)
{
	int Len = static_cast<int>(convert(logBuffer.current(), value));
	logBuffer.appendLen(Len);
	return *this;
}

LogStream &LogStream::operator<<(unsigned short value)
{
	int Len = static_cast<int>(convert(logBuffer.current(), value));
	logBuffer.appendLen(Len);
	return *this;
}

LogStream &LogStream::operator<<(int value)
{
	int Len = static_cast<int>(convert(logBuffer.current(), value));
	logBuffer.appendLen(Len);
	return *this;
}

LogStream &LogStream::operator<<(unsigned int value)
{
	int Len = static_cast<int>(convert(logBuffer.current(), value));
	logBuffer.appendLen(Len);
	return *this;
}

LogStream &LogStream::operator<<(long value)
{
	int Len = static_cast<int>(convert(logBuffer.current(), value));
	logBuffer.appendLen(Len);
	return *this;
}

LogStream &LogStream::operator<<(unsigned long value)
{
	int Len = static_cast<int>(convert(logBuffer.current(), value));
	logBuffer.appendLen(Len);
	return *this;
}

LogStream &LogStream::operator<<(long long value)
{
	int Len = static_cast<int>(convert(logBuffer.current(), value));
	logBuffer.appendLen(Len);
	return *this;
}

LogStream &LogStream::operator<<(unsigned long long value)
{
	int Len = static_cast<int>(convert(logBuffer.current(), value));
	logBuffer.appendLen(Len);
	return *this;
}

LogStream &LogStream::operator<<(float fValue)
{
	int Len = static_cast<int>(snprintf(logBuffer.current(), iFloatMaxStrLen, "%f", fValue));
	logBuffer.appendLen(Len);
	return *this;
}

LogStream &LogStream::operator<<(double dValue)
{
	int Len = static_cast<int>(snprintf(logBuffer.current(), iDoubleMaxStrLen, "%lf", dValue));
	logBuffer.appendLen(Len);
	return *this;
}

LogStream &LogStream::operator<<(char *pChar)
{
	logBuffer.appendString(pChar, static_cast<int>(strlen(pChar)));
	return *this;
}

LogStream &LogStream::operator<<(const char *pChar)
{
	logBuffer.appendString(pChar, static_cast<int>(strlen(pChar)));
	return *this;
}

LogStream &LogStream::operator<<(unsigned char *pChar)
{
	return operator<<(reinterpret_cast<char *> (pChar));
}

LogStream &LogStream::operator<<(const unsigned char *pChar)
{
	return operator<<(reinterpret_cast<const char *> (pChar));
}

LogStream &LogStream::operator<<(std::string strString)
{
	return operator<<(strString.c_str());
}
