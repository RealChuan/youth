#include "CountDownLatch.h"

using namespace youth;

CountDownLatch::CountDownLatch(int count_)
    :mutex()
    ,condition(mutex)
    ,count(count_)
{
}

void CountDownLatch::wait()
{
    MutexLock lock(mutex);
    while(count > 0)
        condition.wait();
}

void CountDownLatch::countDown()
{
    MutexLock lock(mutex);
    count--;
    if(count == 0)
    {
        condition.notifyAll();
    }
}

int CountDownLatch::getCount() const
{
    MutexLock lock(mutex);
    return count;
}
