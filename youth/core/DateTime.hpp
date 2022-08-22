#pragma once

#include "Object.h"

#include <chrono>
#include <string>
#include <string_view>

namespace youth {

namespace core {

// 不同类型的time_point转化为system_clock
template<typename T>
std::chrono::system_clock::time_point toSystemClockTimePoint(T t)
{
    return t - T::clock::now() + std::chrono::system_clock::now();
}

class DateTime : copyable
{
public:
    DateTime()
        : m_microSecondsSinceEpoch(0)
    {}

    explicit DateTime(int64_t microSecondsSinceEpoch)
        : m_microSecondsSinceEpoch(microSecondsSinceEpoch)
    {}

    DateTime(const DateTime &other) { m_microSecondsSinceEpoch = other.m_microSecondsSinceEpoch; }

    DateTime(DateTime &&other) { m_microSecondsSinceEpoch = other.m_microSecondsSinceEpoch; }

    DateTime &operator=(const DateTime &other)
    {
        m_microSecondsSinceEpoch = other.m_microSecondsSinceEpoch;
        return *this;
    }

    DateTime &operator=(DateTime &&other)
    {
        m_microSecondsSinceEpoch = other.m_microSecondsSinceEpoch;
        return *this;
    }

    ~DateTime() {}

    bool operator<(const DateTime &other) const
    {
        return m_microSecondsSinceEpoch < other.m_microSecondsSinceEpoch;
    }

    bool operator<=(const DateTime &other) const
    {
        return m_microSecondsSinceEpoch <= other.m_microSecondsSinceEpoch;
    }

    bool operator>(const DateTime &other) const
    {
        return m_microSecondsSinceEpoch > other.m_microSecondsSinceEpoch;
    }

    bool operator>=(const DateTime &other) const
    {
        return m_microSecondsSinceEpoch >= other.m_microSecondsSinceEpoch;
    }

    bool operator==(const DateTime &other) const
    {
        return m_microSecondsSinceEpoch == other.m_microSecondsSinceEpoch;
    }

    bool operator!=(const DateTime &other) const
    {
        return m_microSecondsSinceEpoch != other.m_microSecondsSinceEpoch;
    }

    void swap(DateTime &that)
    {
        std::swap(m_microSecondsSinceEpoch, that.m_microSecondsSinceEpoch);
    }

    bool isValid() const { return m_microSecondsSinceEpoch > 0; }

    int64_t microSecondsSinceEpoch() const { return m_microSecondsSinceEpoch; }
    int64_t milliSecondsSinceEpoch() const { return m_microSecondsSinceEpoch / 1000; }
    int64_t secondsSinceEpoch() const { return m_microSecondsSinceEpoch / (1000 * 1000); }

    std::string toStandardFormat() const;
    // %Y-%m-%d %H:%M:%S
    std::string toString(const std::string &fmt) const { return toString(fmt.data()); }
    std::string toString(const std::string_view &fmt) const { return toString(fmt.data()); }
    std::string toString(const char *fmt) const;

    inline DateTime addMicroSeconds(int64_t microseconds) const
    {
        return DateTime(m_microSecondsSinceEpoch + microseconds);
    }
    inline DateTime addMilliSeconds(int64_t milliseconds) const
    {
        return addMicroSeconds(milliseconds * 1000);
    }
    inline DateTime addSeconds(int64_t seconds) const { return addMilliSeconds(seconds * 1000); }
    inline DateTime addMinutes(int64_t minutes) const { return addSeconds(minutes * 60); }
    inline DateTime addHours(int64_t hours) const { return addMinutes(hours * 60); }
    inline DateTime addDays(int64_t days) const { return addHours(days * 24); }
    inline DateTime addWeeks(int64_t weeks) const { return addDays(weeks * 7); }
    DateTime addMonths(int64_t months) const;
    DateTime addYears(int64_t years) const;

    static DateTime currentDateTime();
    static int64_t currentMicroSecondsSinceEpoch();
    static int64_t currentMilliSecondsSinceEpoch();
    static int64_t currentSecondsSinceEpoch();
    static DateTime fromString(const std::string &dateTime, const std::string_view &format)
    {
        return fromString(dateTime.data(), format.data());
    }
    static DateTime fromString(std::string_view dateTime, std::string_view format)
    {
        return fromString(dateTime.data(), format.data());
    }
    static DateTime fromString(const std::string &dateTime, std::string_view format)
    {
        return fromString(dateTime.data(), format.data());
    }
    static DateTime fromString(const char *dateTime, const char *format);

    static const int kMicroSecondsPerSecond = 1000 * 1000;

private:
    int64_t m_microSecondsSinceEpoch = 0;
};

} // namespace core

} // namespace youth