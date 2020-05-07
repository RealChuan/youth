/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   YThreadPool.cpp
 * Author: root
 *
 * Created on 2019年8月18日, 下午1:19
 */

#include "ThreadPool.h"

#include <assert.h>

using namespace youth;

ThreadPool::ThreadPool()
	:maxTaskNum_(0)
	,running_(false)
	,mutex_()
	,notEmpty_(mutex_)
	,notFull_(mutex_)
{
}

ThreadPool::~ThreadPool()
{
	if (running_)
	{
		stop();
	}
}

void ThreadPool::start(int numThreads)
{
	assert(threads_.empty());
	running_ = true;
	threads_.reserve(static_cast<unsigned long>(numThreads));
	for (int i = 0; i < numThreads; ++i)
	{
		char id[32];
		snprintf(id, sizeof id, "%d", i + 1);
		threads_.emplace_back(new youth::Thread(
								  std::bind(&ThreadPool::threadFunc, this)));
		threads_[static_cast<unsigned long>(i)]->start();
	}
	if (numThreads == 0 && threadInitCallback_)
	{
		threadInitCallback_();
	}
}

void ThreadPool::stop()
{
	{
		MutexLock lock(mutex_);
		running_ = false;
		notEmpty_.notifyAll();
	}
	for (auto& thr : threads_)
	{
		thr->join();
	}
}

void ThreadPool::threadFunc()
{
	assert(running_ == true);

	if (threadInitCallback_)
	{
		threadInitCallback_();
	}
	while (running_)
	{
		Task task(take());
		if (task)
		{
			task();
		}
	}
}

void ThreadPool::run(Task task)
{
	if (threads_.empty())
	{
		task();
	}
	else
	{
		MutexLock lock(mutex_);
		while (isFull())
		{
			notFull_.wait();
		}
		assert(!isFull());

		queue_.push_back(std::move(task));
		notEmpty_.notify();
	}
}

ThreadPool::Task ThreadPool::take()
{
	MutexLock lock(mutex_);
	// always use a while-loop, due to spurious wakeup
	while (queue_.empty() && running_)
	{
		notEmpty_.wait();
	}
	Task task;
	if (!queue_.empty())
	{
		task = queue_.front();
		queue_.pop_front();
		if (maxTaskNum_ > 0)
		{
			notFull_.notify();
		}
	}
	return task;
}
