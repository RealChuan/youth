/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   YThread.cpp
 * Author: root
 *
 * Created on 2019年8月5日, 下午7:54
 */

#include "Thread.h"
#include "LogOut.h"
#include "assert.h"
#include "Time.h"

using namespace youth;

Thread::Thread(ThreadFunc func_)
	: _started(false),
	  joined(false),
	  pthreadId(0),
	  func(std::move(func_))
{
}

Thread::~Thread()
{
	if (_started && !joined)
	{
		pthread_detach(pthreadId);
	}
}

void Thread::start()
{
	assert(!_started);
	_started = true;
	// FIXME: move(func_)
	//this?????????????????
	if (pthread_create(&pthreadId, nullptr, threadFunc, reinterpret_cast<void*>(this)))
	{
		_started = false;
		LOG_FATAL << "Failed in pthread_create";
		perror("Failed in pthread_create");
		exit(-1);
	}
}

int Thread::join()
{
	assert(_started);
	assert(!joined);
	joined = true;
	return pthread_join(pthreadId, nullptr);
}

//-----------------------------------------------------------

Mutex::Mutex()
{
	pthread_mutex_init(&mutex, nullptr);
}

Mutex::~Mutex()
{
	pthread_mutex_destroy(&mutex);
}

void Mutex::lock()
{
	pthread_mutex_lock(&mutex);
}

void Mutex::unlock()
{
	pthread_mutex_unlock(&mutex);
}

pthread_mutex_t* Mutex::getMutex()
{
	return &mutex;
}

//-------------------------------------------------------

MutexLock::MutexLock(Mutex& mutex_)
	: mutex(mutex_)
{
	mutex.lock();
}

MutexLock::~MutexLock()
{
	mutex.unlock();
}
//-----------------------------------------------------------

Condition::Condition(Mutex& mutex)
	: mutex(mutex)
{
	pthread_cond_init(&cond, nullptr);
}

Condition::~Condition()
{
	pthread_cond_destroy(&cond);
}

void Condition::wait()
{
	pthread_cond_wait(&cond, mutex.getMutex());
}

void Condition::notify()
{
	pthread_cond_signal(&cond);
}

void Condition::notifyAll()
{
	pthread_cond_broadcast(&cond);
}

bool Condition::waitForSeconds(double seconds)
{
	struct timespec abstime;
	// FIXME: use CLOCK_MONOTONIC or CLOCK_MONOTONIC_RAW to prevent time rewind.
	clock_gettime(CLOCK_REALTIME, &abstime);

	const int64_t kNanoSecondsPerSecond = 1000000000;
	int64_t nanoseconds = static_cast<int64_t> (seconds * kNanoSecondsPerSecond);

	abstime.tv_sec += static_cast<time_t> ((abstime.tv_nsec + nanoseconds) / kNanoSecondsPerSecond);
	abstime.tv_nsec = static_cast<long> ((abstime.tv_nsec + nanoseconds) % kNanoSecondsPerSecond);

	return ETIMEDOUT == pthread_cond_timedwait(&cond, mutex.getMutex(), &abstime);
}
