#ifndef EPOLL_H
#define EPOLL_H

#include <vector>
#include <map>

#include "youth/core/Object.h"
#include "youth/core/Timestamp.h"

struct epoll_event;

namespace youth
{

class EventLoop;
class Channel;
class Epoll : public noncopyable
{
    typedef std::vector<Channel*> ChannelList;
public:
    Epoll(EventLoop* eventLoop);
    ~Epoll();

    Timestamp poll(int timeoutMs, ChannelList* activeChannels);

private:
    void fillActiveChannels(int numEvents, ChannelList* activeChannels) const;

    static const int kInitEventListSize = 16;

    EventLoop* m_ownerLoop;

    typedef std::map<int, Channel*> ChannelMap;
    ChannelMap channelMap;

    typedef std::vector<struct epoll_event> EventVec;
    EventVec m_events;
    int m_epollfd;
};
}
#endif // EPOLL_H
