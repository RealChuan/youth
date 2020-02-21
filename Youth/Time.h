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

    //Time now();
    void now();
    std::string getDayToString();
    std::string getSecondToString();
    std::string getMSToString();
    int32_t getAbsTimespec(struct timespec *ts, int32_t millisecond);

private:
    inline void getTime();
    int64_t microSecondsSinceEpoch_;
    struct tm tm_time;
};
}

#endif /* TIME_H */
