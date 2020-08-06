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

}

#endif // CONDITION_H
