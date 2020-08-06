#ifndef MUTEX_H
#define MUTEX_H

#include "Object.h"

#include <pthread.h>

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

}

}

#endif // MUTEX_H
