#ifndef TIME_H
#define TIME_H

#include <string>

namespace youth
{
class Time
{
public:
    explicit Time();
    explicit Time(int64_t);

    static Time now();
    std::string getDayToString();
    std::string getSecondToString();
    std::string getMSToString();
    int32_t getAbsTimespec(struct timespec *ts, int32_t millisecond);

    static const int kMicroSecondsPerSecond = 1000 * 1000;
private:
    inline void now_();
    inline bool getTime();
    int64_t mcroSecondsSinceEpoch;
    struct tm tm_time;
    time_t lastSecond;
};

}

#endif /* TIME_H */
