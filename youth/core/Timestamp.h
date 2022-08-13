#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <string>

#include "Object.h"

namespace youth {

namespace core {

class Timestamp : copyable
{
public:
    explicit Timestamp()
        : m_mcroSecondsSinceEpoch(0)
        , m_lastSecond(0)
    {}

    explicit Timestamp(int64_t ms)
        : m_mcroSecondsSinceEpoch(ms)
        , m_lastSecond(0)
    {}

    void swap(Timestamp &that) { std::swap(m_mcroSecondsSinceEpoch, that.m_mcroSecondsSinceEpoch); }

    bool isValid() const { return m_mcroSecondsSinceEpoch > 0; }

    int64_t microSecondsSinceEpoch() const { return m_mcroSecondsSinceEpoch; }
    time_t secondsSinceEpoch() const
    {
        return time_t(m_mcroSecondsSinceEpoch / kMicroSecondsPerSecond);
    }

    static Timestamp currentTimestamp();
    static Timestamp invalid() { return Timestamp(); }

    static Timestamp fromUnixTime(time_t t) { return fromUnixTime(t, 0); }
    static Timestamp fromUnixTime(time_t t, int microseconds)
    {
        return Timestamp(int64_t(t) * kMicroSecondsPerSecond + microseconds);
    }

    std::string getDayToString();
    std::string getSecondToString();
    std::string microSecondsToString();

    static const int kMicroSecondsPerSecond = 1000 * 1000;

private:
    inline bool getTime();

    int64_t m_mcroSecondsSinceEpoch;
    struct tm m_tmTime;
    time_t m_lastSecond;
};

inline bool operator<(Timestamp lhs, Timestamp rhs)
{
    return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator==(Timestamp lhs, Timestamp rhs)
{
    return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}

inline double timeDifference(Timestamp high, Timestamp low)
{
    int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
    return double(diff) / Timestamp::kMicroSecondsPerSecond;
}

inline Timestamp addTime(Timestamp timestamp, double seconds)
{
    int64_t delta = int64_t(seconds * Timestamp::kMicroSecondsPerSecond);
    return Timestamp(timestamp.microSecondsSinceEpoch() + delta);
}

} // namespace core

} // namespace youth

#endif /* TIMESTAMP_H */
