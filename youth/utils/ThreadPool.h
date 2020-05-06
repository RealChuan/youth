/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   YThreadPool.h
 * Author: root
 *
 * Created on 2019年8月18日, 下午1:19
 */

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <deque>
#include <memory>
#include <boost/function.hpp>

#include "Thread.h"
#include "youth/core/Object.h"

namespace youth
{
class ThreadPool : public noncopyable
{
	typedef std::function<void () > Task;

public:
	ThreadPool();

	virtual ~ThreadPool();

	void setTaskNum(int maxTaskNum)
	{
		maxTaskNum_ = maxTaskNum;
	}

	void setThreadInitCallback(const Task& cb)
	{
		threadInitCallback_ = cb;
	}

	void start(int numThreads);
	void stop();

	size_t queueSize() const
	{
		//YMutexLock lock(mutex_);
		return queue_.size();
	}

	void run(Task f);

private:

	bool isFull() const REQUIRES(mutex_)
	{
		MutexLock lock(mutex_);
		return maxTaskNum_ > 0 && queue_.size() >= maxTaskNum_;
	}

	void threadFunc();
	Task take();

	Task threadInitCallback_;
	size_t maxTaskNum_;
	bool running_;
	mutable Mutex mutex_;

	Condition notEmpty_ GUARDED_BY(mutex_);
	Condition notFull_ GUARDED_BY(mutex_);

	std::vector<std::unique_ptr<youth::Thread>> threads_;
	std::deque<Task> queue_ GUARDED_BY(mutex_);

};
}

#endif /* THREADPOOL_H */

