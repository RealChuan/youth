#ifndef EPOLL_H
#define EPOLL_H

#include <vector>

#include "youth/core/Object.h"
#include "youth/core/Timestamp.h"

struct epoll_event;

namespace youth
{
class Epoll : public noncopyable
{
public:
    Epoll();
    ~Epoll();

    Timestamp poll(int timeoutMs);

private:
    typedef std::vector<struct epoll_event> EventVec;
    EventVec m_events;
    int m_epollfd;
};
}
#endif // EPOLL_H
