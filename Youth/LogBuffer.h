/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LogBuffer.h
 * Author: root
 *
 * Created on 2019年8月4日, 上午6:50
 */

#ifndef LOGBUFFER_H
#define LOGBUFFER_H

#include <string.h>
#include <algorithm>
#include <stdint.h>

namespace youth
{

//下面这个类用来作为一个固定缓存，这个缓存将输出的日志信息
//追加到缓存区中，以便最后一条完整日志的输出。

const int normalBuffer = 4000; //可以使用的正常的缓存大小
const int maxBuffer = 4000 * 1000; //可以使用的最大的缓存大小

template<int SIZE>
class LogBuffer
{
public:
	LogBuffer()
		: p_myData(myData)
	{
		memset(myData, 0, SIZE);
	}

	~LogBuffer()
	{
		//nothing
	}

	void reset()
	{
		p_myData = myData;
	}

	void bzero()
	{
		memset(myData, 0, sizeof myData);
	}

	void append(const char *pData, int Len)
	{
		char *pEndBuffer = &myData[SIZE - 1];
		if (static_cast<int> (pEndBuffer - p_myData) > Len)
		{
			memcpy(p_myData, pData, Len);
			p_myData += Len;
		}
	}

	//这个方法是用来获取buffer数据
	const char *data()
	{
		return myData;
	}

	//这个方法是用来获取buffer的长度
	int length()
	{
		return static_cast<int> (p_myData - myData);
	}

	//这个方法是用来将某字符串追加到buffer中
	void appendString(const char *pString, int Len)
	{
		memcpy(p_myData, pString, Len);
		p_myData += Len;
	}

	//获取剩余可写长度
	int avail() const
	{
		return static_cast<int> (end() - p_myData);
	}

	//这个方法是用来返回buffer当前的指针位置，
	//以便外部能直接操作buffer，提高效率
	char *current(void)
	{
		return p_myData;
	}

	//这个方法与current配套使用
	void appendLen(int Len)
	{
		p_myData += Len;
	}

private:
	const char* end() const
	{
		return myData + sizeof myData;
	}

	char myData[SIZE];
	char *p_myData;
};

template class LogBuffer<normalBuffer>;
template class LogBuffer<maxBuffer>;
}

#endif /* LOGBUFFER_H */
