#include "Timer.h"

namespace youth
{

namespace net
{

std::atomic<int64_t> Timer::m_numCreated = 0;

Timer::Timer(TimerCallback cb, Timestamp when, double interval)
    : m_callback(std::move(cb))
    , m_expiration(when)
    , m_interval(interval)
    , m_repeat(interval > 0.0)
    , m_sequence(m_numCreated.fetch_add(1))
{
}

void Timer::run() const
{
    m_callback();
}

Timestamp Timer::expiration() const
{
    return m_expiration;
}

bool Timer::repeat() const
{
    return m_repeat;
}

int64_t Timer::sequence() const
{
    return m_sequence;
}

void Timer::restart(Timestamp now)
{
    if (m_repeat)
    {
        m_expiration = addTime(now, m_interval);
    }
    else
    {
        m_expiration = Timestamp::invalid();
    }
}

int64_t Timer::numCreated()
{
    return m_numCreated;
}

}

}
