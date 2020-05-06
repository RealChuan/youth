#ifndef ELAPSEDTIME_H
#define ELAPSEDTIME_H

#include <string>
#include "youth/core/Object.h"

namespace  youth
{

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
