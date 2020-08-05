#ifndef ELAPSEDTIME_H
#define ELAPSEDTIME_H

#include <string>

#include "youth/core/Object.h"

namespace  youth
{

class ElapsedTime : noncopyable
{
public:
    ElapsedTime();

    void start();
    void reStart();
    std::string elapsed();

private:
    struct timeval m_tvStart;
};

}

#endif // ELAPSEDTIME_H
