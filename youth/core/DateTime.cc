#include "DateTime.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string.h>

namespace youth {

namespace core {

std::string DateTime::toStandardFormat() const
{
    char buf[32] = {0};
    std::chrono::time_point<std::chrono::system_clock> tp(
        (std::chrono::microseconds(m_microSecondsSinceEpoch)));
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
    auto usec = m_microSecondsSinceEpoch % 1000000;
    std::snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), ".%06d", static_cast<int>(usec));
    return buf;
}

std::string DateTime::toString(const char *fmt) const
{
    char buf[32] = {0};
    std::chrono::time_point<std::chrono::system_clock> tp(
        (std::chrono::microseconds(m_microSecondsSinceEpoch)));
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::strftime(buf, sizeof(buf), fmt, std::localtime(&t));
    return buf;
}

DateTime DateTime::addMonths(int64_t months) const
{
    std::chrono::time_point<std::chrono::system_clock> tp(
        (std::chrono::microseconds(m_microSecondsSinceEpoch)));
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm = *std::gmtime(&t);
    tm.tm_mon += months;
    tm.tm_year += tm.tm_mon / 12;
    tm.tm_mon %= 12;
    tm.tm_mday = std::min(tm.tm_mday,
                          tm.tm_mon == 1 ? 31
                                         : (tm.tm_mon == 3 || tm.tm_mon == 5 || tm.tm_mon == 8
                                                    || tm.tm_mon == 10
                                                ? 30
                                                : (tm.tm_mon == 4 || tm.tm_mon == 6
                                                           || tm.tm_mon == 9 || tm.tm_mon == 11
                                                       ? 29
                                                       : 28)));
    t = std::mktime(&tm);
    return DateTime(std::chrono::system_clock::from_time_t(t).time_since_epoch().count() / 1000
                    + m_microSecondsSinceEpoch % 1000000);
}

DateTime DateTime::addYears(int64_t years) const
{
    std::chrono::time_point<std::chrono::system_clock> tp(
        (std::chrono::microseconds(m_microSecondsSinceEpoch)));
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm = *std::gmtime(&t);
    tm.tm_year += years;
    if (tm.tm_mon == 2 && (tm.tm_year + 1900) % 4 > 0) {
        tm.tm_mday = std::min(tm.tm_mday, 28);
    }
    t = std::mktime(&tm);
    return DateTime(std::chrono::system_clock::from_time_t(t).time_since_epoch().count() / 1000
                    + m_microSecondsSinceEpoch % 1000000);
}

DateTime DateTime::currentDateTime()
{
    auto now = std::chrono::system_clock::now();
    auto microSeconds
        = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
    return DateTime(microSeconds);
}

int64_t DateTime::currentMicroSecondsSinceEpoch()
{
    return std::chrono::duration_cast<std::chrono::microseconds>(
               std::chrono::system_clock::now().time_since_epoch())
        .count();
}

int64_t DateTime::currentMilliSecondsSinceEpoch()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::system_clock::now().time_since_epoch())
        .count();
}

int64_t DateTime::currentSecondsSinceEpoch()
{
    return std::chrono::duration_cast<std::chrono::seconds>(
               std::chrono::system_clock::now().time_since_epoch())
        .count();
}

DateTime DateTime::fromString(const std::string &dateTime, const char *format)
{
    std::tm tm = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::istringstream is(dateTime);
    is >> std::get_time(&tm, format);
    auto t = std::mktime(&tm);
    return DateTime(std::chrono::system_clock::from_time_t(t).time_since_epoch().count() / 1000);
}

} // namespace core

} // namespace youth