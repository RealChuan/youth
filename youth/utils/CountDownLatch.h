#ifndef COUNTDOWNLATCH_H
#define COUNTDOWNLATCH_H

#include "Thread.h"
#include "youth/core/Object.h"


namespace youth
{

class CountDownLatch : noncopyable
{
public:
    CountDownLatch(int count_);
    void wait();
    void countDown();
    int getCount() const;

private:
    mutable Mutex mutex;
    Condition condition GUARDED_BY(mutex);
    int count GUARDED_BY(mutex);
};

}

#endif // COUNTDOWNLATCH_H
