#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <string>

#include "Object.h"

namespace youth
{

namespace core
{

class Timestamp : copyable
{
public:
    explicit Timestamp();
    explicit Timestamp(int64_t);

    void swap(Timestamp& that);

    bool isValid() const;

    int64_t microSecondsSinceEpoch() const;
    time_t secondsSinceEpoch() const;

    static Timestamp currentTimestamp();
    static Timestamp invalid();

    static Timestamp fromUnixTime(time_t t);
    static Timestamp fromUnixTime(time_t t, int microseconds);

    std::string getDayToString();
    std::string getSecondToString();
    std::string microSecondsToString();
    int32_t getAbsTimespec(struct timespec *ts, int32_t millisecond);

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

}

}

#endif /* TIMESTAMP_H */
