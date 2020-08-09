#include "TimerId.h"

namespace youth
{

namespace net
{

TimerId::TimerId()
{
}

TimerId::TimerId(Timer *timer, int64_t seq)
    : m_timer(timer)
    , m_sequence(seq)
{
}

}

}
