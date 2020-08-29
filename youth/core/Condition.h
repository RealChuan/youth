#ifndef CONDITION_H
#define CONDITION_H

#include "Mutex.h"

namespace youth
{

namespace core
{

class Condition : noncopyable
{
public:
    Condition(Mutex& mutex)
        : m_mutex(mutex)
    {
        pthread_cond_init(&m_cond, nullptr);
    }

    ~Condition()
    {
        pthread_cond_destroy(&m_cond);
    }

    void wait()
    {
        pthread_cond_wait(&m_cond, m_mutex.getMutex());
    }

    void notify()
    {
        pthread_cond_signal(&m_cond);
    }

    void notifyAll()
    {
        pthread_cond_broadcast(&m_cond);
    }

    bool waitForSeconds(double seconds);

private:
    Mutex& m_mutex;
    pthread_cond_t m_cond;
};

}

}

#endif // CONDITION_H
