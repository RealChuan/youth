#ifndef EPOLL_H
#define EPOLL_H

#include <map>
#include <vector>

#include <youth/core/DateTime.hpp>

struct epoll_event;

namespace youth {

using namespace core;

namespace net {

class EventLoop;
class Channel;
class Epoll : noncopyable
{
public:
    using ChannelList = std::vector<Channel *>;

    Epoll(EventLoop *eventLoop);
    ~Epoll();

    DateTime poll(int timeoutMs, ChannelList *activeChannels);
    void updateChannel(Channel *channel);
    void removeChannel(Channel *channel);

    bool hasChannel(Channel *channel) const;

private:
    void fillActiveChannels(int numEvents, ChannelList *activeChannels) const;
    void update(int operation, Channel *channel);

    static const int kInitEventListSize = 16;

    EventLoop *m_eventLoop;
    typedef std::map<int, Channel *> ChannelMap;
    ChannelMap m_channelMap;
    std::vector<struct epoll_event> m_eventVec;
    int m_epollfd;
};

} // namespace net

} // namespace youth
#endif // EPOLL_H
