#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <string>

namespace youth
{
class Timestamp
{
public:
    explicit Timestamp();
    explicit Timestamp(int64_t);

    static Timestamp now();
    std::string getDayToString();
    std::string getSecondToString();
    std::string getMicroSToString();
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

#endif /* TIMESTAMP_H */