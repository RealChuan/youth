#ifndef MUTEX_H
#define MUTEX_H

#include "Object.h"
#include "CurrentThread.h"

#include <pthread.h>
#include <assert.h>

#if defined(__clang__) && (!defined(SWIG))
#define THREAD_ANNOTATION_ATTRIBUTE__(x)   __attribute__((x))
#else
#define THREAD_ANNOTATION_ATTRIBUTE__(x)   // no-op
#endif

#define GUARDED_BY(x) THREAD_ANNOTATION_ATTRIBUTE__(guarded_by(x))
#define REQUIRES(...) THREAD_ANNOTATION_ATTRIBUTE__(requires_capability(__VA_ARGS__))

namespace youth
{

namespace core
{

class Mutex : noncopyable
{
public:
    Mutex()
    {
        pthread_mutex_init(&m_mutex, nullptr);
    }
    ~Mutex()
    {
        assert(hodler == 0);
        pthread_mutex_destroy(&m_mutex);
    }

    void lock()
    {
        hodler = CurrentThread::tid();
        pthread_mutex_lock(&m_mutex);
    }
    void unlock()
    {
        hodler = 0;
        pthread_mutex_unlock(&m_mutex);
    }

    pthread_mutex_t* getMutex()
    {
        return &m_mutex;
    }

private:
    pthread_mutex_t m_mutex;
    pid_t hodler = 0;
};

class MutexLock : noncopyable
{
public:
    MutexLock(Mutex& mutex_)
        : m_mutex(mutex_)
    {
        m_mutex.lock();
    }

    ~MutexLock()
    {
        m_mutex.unlock();
    }

private:
    Mutex& m_mutex;
};

}

}

#endif // MUTEX_H
