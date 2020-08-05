#ifndef COUNTDOWNLATCH_H
#define COUNTDOWNLATCH_H

#include "Thread.h"
#include "youth/core/Object.h"


namespace youth
{

class CountDownLatch : noncopyable
{
public:
    CountDownLatch(int count);
    void wait();
    void countDown();
    int getCount() const;

private:
    mutable Mutex m_mutex;
    Condition m_condition GUARDED_BY(m_mutex);
    int m_count GUARDED_BY(m_mutex);
};

}

#endif // COUNTDOWNLATCH_H
