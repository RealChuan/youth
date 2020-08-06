#include "Mutex.h"

namespace youth
{

namespace core
{

Mutex::Mutex()
{
    pthread_mutex_init(&m_mutex, nullptr);
}

Mutex::~Mutex()
{
    pthread_mutex_destroy(&m_mutex);
}

void Mutex::lock()
{
    pthread_mutex_lock(&m_mutex);
}

void Mutex::unlock()
{
    pthread_mutex_unlock(&m_mutex);
}

pthread_mutex_t* Mutex::getMutex()
{
    return &m_mutex;
}

/*
 *
 */

MutexLock::MutexLock(Mutex& mutex_)
    : m_mutex(mutex_)
{
    m_mutex.lock();
}

MutexLock::~MutexLock()
{
    m_mutex.unlock();
}
}

}
