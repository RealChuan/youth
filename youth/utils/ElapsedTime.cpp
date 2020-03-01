#include "ElapsedTime.h"
#include "../core/Timestamp.h"

#include <sys/time.h>

using namespace youth;

ElapsedTime::ElapsedTime()
{

}

void ElapsedTime::start()
{
    gettimeofday(&tv_start, NULL);
}

void ElapsedTime::reStart()
{
    gettimeofday(&tv_start, NULL);
}

std::string ElapsedTime::elapsed()
{
    struct timeval tv_end;
    gettimeofday(&tv_end, NULL);
    int t = Timestamp::kMicroSecondsPerSecond * (tv_end.tv_sec - tv_start.tv_sec) +
            (tv_end.tv_usec - tv_start.tv_usec);
    char buf[20];
    double seconds = t * 1.0 / Timestamp::kMicroSecondsPerSecond;
    snprintf(buf, sizeof buf, "%.6lf(S)", seconds);
    return buf;
}
