#ifndef COUNTDOWNLATCH_H
#define COUNTDOWNLATCH_H

#include "Thread.hpp"

namespace youth {

namespace core {

class CountDownLatch : noncopyable
{
public:
    CountDownLatch(int count);
    void wait();
    void countDown();
    int getCount() const;

private:
    mutable Mutex m_mutex;
    Condition m_condition;
    int m_count;
};

} // namespace core

} // namespace youth

#endif // COUNTDOWNLATCH_H
