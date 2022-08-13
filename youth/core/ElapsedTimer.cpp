#include "ElapsedTimer.h"

#include <iostream>

namespace youth {

namespace core {

std::string ElapsedTimer::elapsed() const
{
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - m_startPoint;
    char buf[20];
    snprintf(buf, sizeof buf, "%.6lf(S)", elapsed_seconds.count());
    return buf;
}

bool ElapsedTimer::hasExpired(int64_t microseconds) const
{
    auto end = std::chrono::steady_clock::now();
    std::chrono::microseconds elapsed_microseconds
        = std::chrono::duration_cast<std::chrono::microseconds>(end - m_startPoint);
    return elapsed_microseconds.count() > microseconds;
}

} // namespace core

} // namespace youth
