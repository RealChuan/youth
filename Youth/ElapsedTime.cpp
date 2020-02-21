#include "ElapsedTime.h"
#include "Time.h"

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
    int t = Time::kMicroSecondsPerSecond * (tv_end.tv_sec - tv_start.tv_sec) +
            (tv_end.tv_usec - tv_start.tv_usec);
    std::string str;
    char buf[32];
    int seconds = t / Time::kMicroSecondsPerSecond;
    if(seconds > 0){
        snprintf(buf, sizeof buf, "%ds ", seconds);
        str += buf;
    }
    snprintf(buf, sizeof buf, "%dus", t % (1000 * 1000));
    str += buf;
    return str;
}
