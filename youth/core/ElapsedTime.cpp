#include "ElapsedTime.h"
#include "Timestamp.h"

#include <sys/time.h>

namespace youth
{

namespace core
{

ElapsedTime::ElapsedTime()
{
}

void ElapsedTime::start()
{
    gettimeofday(&m_tvStart, NULL);
}

void ElapsedTime::reStart()
{
    gettimeofday(&m_tvStart, NULL);
}

std::string ElapsedTime::elapsed()
{
    struct timeval tv_end;
    gettimeofday(&tv_end, NULL);
    long int t = Timestamp::kMicroSecondsPerSecond * (tv_end.tv_sec - m_tvStart.tv_sec)
            + (tv_end.tv_usec - m_tvStart.tv_usec);
    char buf[20];
    double seconds = double(t) / Timestamp::kMicroSecondsPerSecond;
    snprintf(buf, sizeof buf, "%.6lf(S)", seconds);
    return buf;
}

}

}
