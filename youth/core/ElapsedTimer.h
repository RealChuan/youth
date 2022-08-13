#pragma once

#include <chrono>
#include <string>

#include "Object.h"

namespace youth {

namespace core {

class ElapsedTimer : noncopyable
{
public:
    ElapsedTimer() {}

    void start() { m_startPoint = std::chrono::steady_clock::now(); }
    void reStart() { start(); }
    std::string elapsed() const;

    bool hasExpired(int64_t microseconds) const;

    bool isVaid() const { return m_startPoint.time_since_epoch().count() > 0; }

private:
    std::chrono::steady_clock::time_point m_startPoint;
};

} // namespace core

} // namespace youth
