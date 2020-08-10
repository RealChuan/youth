#ifndef TIMERID_H
#define TIMERID_H

#include <youth/core/Object.h>

#include <stdint.h>

namespace youth
{

using namespace core;

namespace net
{

class Timer;
class TimerId : copyable
{
public:
    TimerId() {}
    TimerId(Timer* timer, int64_t seq)
        : m_timer(timer)
        , m_sequence(seq)
    {}

    // default copy-ctor, dtor and assignment are okay

    friend class TimerQueue;

private:
    Timer* m_timer = nullptr;
    int64_t m_sequence = 0;
};

}

}

#endif // TIMERID_H
