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

class Thread : noncopyable
{
    typedef std::function<void()> ThreadFunc;
public:
    Thread(ThreadFunc func_);
    ~Thread();

    void start();
    int join(); // return pthread_join()
    bool isRunning() const { return m_running; }
    pthread_t pthreadID() const {return m_pthreadId; }
    static void* threadFunc(void *obj){
        Thread *pThis = static_cast<Thread *> (obj);
        pThis->m_func(); //-------------------------------
        return 0;
    }

private:	
    bool m_running;
    bool m_joined;
    pthread_t m_pthreadId;
    //pid_t _tid;
    ThreadFunc m_func;
    //std::string _name;
};

class Mutex : noncopyable
{
public:
    Mutex();
    ~Mutex();

    void lock();
    void unlock();

    pthread_mutex_t* getMutex();

private:
    pthread_mutex_t m_mutex;
};

class MutexLock : noncopyable
{
public:
    MutexLock(Mutex& mutex_);
    ~MutexLock();

private:
    Mutex& m_mutex;
};

class Condition : noncopyable
{
public:
    Condition(Mutex& m_mutex);
    ~Condition();

    void wait();
    void notify();
    void notifyAll();
    bool waitForSeconds(double seconds);

private:
    Mutex& m_mutex;
    pthread_cond_t m_cond;
};
}

#endif /* THREAD_H */
