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

int daysOfMonth(int year, int month)
{
    if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
            return 29;
        } else {
            return 28;
        }
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    } else {
        return 31;
    }
}

DateTime DateTime::addMonths(int64_t months) const
{
    std::chrono::time_point<std::chrono::system_clock> tp(
        (std::chrono::microseconds(m_microSecondsSinceEpoch)));
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm = *std::localtime(&t);
    tm.tm_mon += months;
    tm.tm_year += tm.tm_mon / 12;
    tm.tm_mon %= 12;
    tm.tm_mday = std::min(tm.tm_mday, daysOfMonth(tm.tm_year + 1900, tm.tm_mon + 1));
    t = std::mktime(&tm);
    auto microSecondsSinceEpoch = std::chrono::duration_cast<std::chrono::microseconds>(
                                      std::chrono::system_clock::from_time_t(t).time_since_epoch())
                                      .count()
                                  + m_microSecondsSinceEpoch % 1000000;
    return DateTime(microSecondsSinceEpoch);
}

DateTime DateTime::addYears(int64_t years) const
{
    std::chrono::time_point<std::chrono::system_clock> tp(
        (std::chrono::microseconds(m_microSecondsSinceEpoch)));
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm = *std::localtime(&t);
    tm.tm_year += years;
    tm.tm_mday = std::min(tm.tm_mday, daysOfMonth(tm.tm_year + 1900, tm.tm_mon + 1));
    t = std::mktime(&tm);
    auto microSecondsSinceEpoch = std::chrono::duration_cast<std::chrono::microseconds>(
                                      std::chrono::system_clock::from_time_t(t).time_since_epoch())
                                      .count()
                                  + m_microSecondsSinceEpoch % 100000;
    return DateTime(microSecondsSinceEpoch);
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

DateTime DateTime::fromString(const char *dateTime, const char *format)
{
    std::tm tm = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::istringstream is(dateTime);
    is >> std::get_time(&tm, format);
    auto t = std::mktime(&tm);
    return DateTime(std::chrono::system_clock::from_time_t(t).time_since_epoch().count() / 1000);
}

} // namespace core

} // namespace youth