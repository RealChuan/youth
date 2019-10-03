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
#include "Thread.h"
#include "Time.h"

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
        thread.start();
		printf("日志线程开始\n");
	}

	void stop()
	{
		running = false;
        cond.notify();
        thread.join();
		printf("日志线程结束\n");
	}

private:
	void threadFunc();
	Time myTime;

	typedef LogBuffer<maxBuffer> Buffer;
	typedef std::vector<std::unique_ptr<Buffer>> BufferVector;
	typedef BufferVector::value_type BufferPtr;

	const int refreshTime;
	bool running;
    Thread thread;
    Mutex mutex;
    Condition cond GUARDED_BY(mutex);
    BufferPtr currentBuffer GUARDED_BY(mutex);
    BufferPtr nextBuffer GUARDED_BY(mutex);
    BufferVector buffers GUARDED_BY(mutex);
};
}
#endif /* LOGASYNC_H */
