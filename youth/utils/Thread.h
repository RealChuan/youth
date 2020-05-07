#ifndef THREAD_H
#define THREAD_H

#include "string"
#include "youth/core/Object.h"

#include <functional>

#if defined(__clang__) && (!defined(SWIG))
#define THREAD_ANNOTATION_ATTRIBUTE__(x)   __attribute__((x))
#else
#define THREAD_ANNOTATION_ATTRIBUTE__(x)   // no-op
#endif

#define GUARDED_BY(x) THREAD_ANNOTATION_ATTRIBUTE__(guarded_by(x))
#define REQUIRES(...) THREAD_ANNOTATION_ATTRIBUTE__(requires_capability(__VA_ARGS__))

namespace youth
{

class Thread : public noncopyable
{
    typedef std::function<void()> ThreadFunc;
public:
    Thread(ThreadFunc func_);
    ~Thread();

    void start();
    int join(); // return pthread_join()
    bool isRunning() const { return running; }
    pthread_t pthreadID() const {return pthreadId; }
    static void* threadFunc(void *obj){
        Thread *pThis = static_cast<Thread *> (obj);
        pThis->func(); //-------------------------------
        return 0;
    }

private:	
    bool running;
    bool joined;
    pthread_t pthreadId;
    //pid_t _tid;
    ThreadFunc func;
    //std::string _name;
};

class Mutex : public noncopyable
{
public:
    Mutex();
    ~Mutex();

    void lock();
    void unlock();

    pthread_mutex_t* getMutex();

private:
    pthread_mutex_t mutex;
};

class MutexLock : public noncopyable
{
public:
    MutexLock(Mutex& mutex_);
    ~MutexLock();

private:
    Mutex& mutex;
};

class Condition : public noncopyable
{
public:
    Condition(Mutex& mutex);
    ~Condition();

    void wait();
    void notify();
    void notifyAll();
    bool waitForSeconds(double seconds);

private:
    Mutex& mutex;
    pthread_cond_t cond;
};
}

#endif /* THREAD_H */
