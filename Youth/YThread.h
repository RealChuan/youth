/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   YThread.h
 * Author: root
 *
 * Created on 2019年8月5日, 下午7:54
 */

#ifndef YTHREAD_H
#define YTHREAD_H

#include "string"
#include "boost/function.hpp"

#if defined(__clang__) && (!defined(SWIG))
#define THREAD_ANNOTATION_ATTRIBUTE__(x)   __attribute__((x))
#else
#define THREAD_ANNOTATION_ATTRIBUTE__(x)   // no-op
#endif

#define GUARDED_BY(x) THREAD_ANNOTATION_ATTRIBUTE__(guarded_by(x))
#define REQUIRES(...) THREAD_ANNOTATION_ATTRIBUTE__(requires_capability(__VA_ARGS__))

namespace youth
{
class YThread
{
	typedef std::function<void() > ThreadFunc;
public:
	YThread(ThreadFunc func_, void* value_);
	virtual ~YThread();

	void start();
	int join(); // return pthread_join()
	bool started() const{return _started;}
	static void* threadFunc(void *obj)
	{
		YThread *pThis = static_cast<YThread *> (obj);
		pThis->func(); //-------------------------------
		return 0;
	}

private:	
	bool _started;
	bool joined;
	pthread_t pthreadId;
	//pid_t _tid;
	ThreadFunc func;
	//std::string _name;
	void *value;

};

class YMutex
{
public:
	YMutex();
	~YMutex();

	void lock();
	void unlock();

	pthread_mutex_t* getMutex();

private:
	pthread_mutex_t mutex;

};

class YMutexLock
{
public:
	YMutexLock(YMutex& mutex);
	~YMutexLock();

private:
	YMutex& mutex;

};

class Condition
{
public:
	Condition(YMutex& mutex);
	~Condition();

	void wait();
	void notify();
	void notifyAll();
	bool waitForSeconds(double seconds);

private:
	YMutex& mutex;
	pthread_cond_t cond;
};
}

#endif /* YTHREAD_H */
