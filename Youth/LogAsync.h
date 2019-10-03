/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LogAsync.h
 * Author: root
 *
 * Created on 2019年8月5日, 下午7:30
 */

#ifndef LOGASYNC_H
#define LOGASYNC_H

#include "LogBuffer.h"
#include "vector"
#include "memory"
#include "string"
#include "YThread.h"
#include "YTime.h"

namespace youth
{
class LogAsync
{
public:
	LogAsync(int flushInterval);
	virtual ~LogAsync();

	void append(const char* logline, int len);

	void start()
	{
		running = true;
		myThread.start();
		printf("日志线程开始\n");
	}

	void stop()
	{
		running = false;
		cond_.notify();
		myThread.join();
		printf("日志线程结束\n");
	}

private:
	void threadFunc();
	YTime myTime;

	typedef LogBuffer<maxBuffer> Buffer;
	typedef std::vector<std::unique_ptr<Buffer>> BufferVector;
	typedef BufferVector::value_type BufferPtr;

	const int refreshTime;
	bool running;
	YThread myThread;
	YMutex myMutex;
	Condition cond_ GUARDED_BY(myMutex);
	BufferPtr currentBuffer_ GUARDED_BY(myMutex);
	BufferPtr nextBuffer_ GUARDED_BY(myMutex);
	BufferVector buffers_ GUARDED_BY(myMutex);
};
}
#endif /* LOGASYNC_H */
