#ifndef TIMER_H
#define TIMER_H

#include <youth/core/Timestamp.h>

#include <atomic>

#include "Callbacks.h"

namespace youth
{

using namespace core;

namespace net
{

class Timer : noncopyable
{
public:
    Timer(TimerCallback cb, Timestamp when, double interval);

    void run() const;

    Timestamp expiration() const;
    bool repeat() const;
    int64_t sequence() const;

    void restart(Timestamp now);

    static int64_t numCreated();

private:
    const TimerCallback m_callback;
    Timestamp m_expiration;
    const double m_interval;
    const bool m_repeat;
    const int64_t m_sequence;

    static std::atomic<int64_t> m_numCreated;
};

}

}

#endif // TIMER_H
