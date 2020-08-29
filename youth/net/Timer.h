#ifndef TIMER_H
#define TIMER_H

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

    void run() const { m_callback(); }

    Timestamp expiration() const { return m_expiration; }
    bool repeat() const { return m_repeat; }
    int64_t sequence() const { return m_sequence; }

    void restart(Timestamp now);

    static int64_t numCreated() { return m_numCreated; }

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
