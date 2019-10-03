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
        : p_data(data_)
	{
        memset(data_, 0, SIZE);
	}

	~LogBuffer()
	{
		//nothing
	}

	void reset()
	{
        p_data = data_;
	}

	void bzero()
	{
        memset(data_, 0, sizeof data_);
	}

	void append(const char *pData, int Len)
	{
        char *pEndBuffer = &data_[SIZE - 1];
        if (static_cast<int> (pEndBuffer - p_data) > Len)
		{
            memcpy(p_data, pData, Len);
            p_data += Len;
		}
	}

	//这个方法是用来获取buffer数据
	const char *data()
	{
        return data_;
	}

	//这个方法是用来获取buffer的长度
	int length()
	{
        return static_cast<int> (p_data - data_);
	}

	//这个方法是用来将某字符串追加到buffer中
	void appendString(const char *pString, int Len)
	{
        memcpy(p_data, pString, Len);
        p_data += Len;
	}

	//获取剩余可写长度
	int avail() const
	{
        return static_cast<int> (end() - p_data);
	}

	//这个方法是用来返回buffer当前的指针位置，
	//以便外部能直接操作buffer，提高效率
	char *current(void)
	{
        return p_data;
	}

	//这个方法与current配套使用
	void appendLen(int Len)
	{
        p_data += Len;
	}

private:
	const char* end() const
	{
        return data_ + sizeof data_;
	}

    char data_[SIZE];
    char *p_data;
};

template class LogBuffer<normalBuffer>;
template class LogBuffer<maxBuffer>;
}

#endif /* LOGBUFFER_H */
