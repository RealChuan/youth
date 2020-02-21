#ifndef ELAPSEDTIME_H
#define ELAPSEDTIME_H

#include <string>

namespace  youth{

class ElapsedTime
{
public:
    ElapsedTime();

    void start();
    void reStart();
    std::string elapsed();

private:
    struct timeval tv_start;
};

}

#endif // ELAPSEDTIME_H
