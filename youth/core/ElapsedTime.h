#ifndef ELAPSEDTIME_H
#define ELAPSEDTIME_H

#include <string>

#include "Object.h"

namespace youth
{

namespace core
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

}

#endif // ELAPSEDTIME_H
