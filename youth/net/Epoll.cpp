#include <sys/epoll.h>
#include <unistd.h>
#include <assert.h>

#include "Epoll.h"
#include "Socket.h"
#include "Channel.h"
#include "youth/utils/LogOut.h"

using namespace youth;

Epoll::Epoll()
    : m_events(kInitEventListSize)
    , m_epollfd(::epoll_create(EPOLL_CLOEXEC))
{

}

Epoll::~Epoll()
{
    ::close(m_epollfd);
}

Timestamp Epoll::poll(int timeoutMs)
{
    int numEvents = ::epoll_wait(m_epollfd, &*m_events.begin(),
                                 static_cast<int>(m_events.size()),
                                 timeoutMs);
    int savedErrno = errno;
    Timestamp timestamp(Timestamp::currentTimestamp());
    if (numEvents > 0)
    {
        LOG_DEBUG << numEvents << " events happened";
        //fillActiveChannels(numEvents, activeChannels);
    }
    else if (numEvents == 0)
    {
        LOG_DEBUG << "nothing happened";
    }
    else
    {
        // error happens, log uncommon ones
        if (savedErrno != EINTR)
        {
            errno = savedErrno;
            LOG_ERROR << "EPollPoller::poll()" << errno;
        }
    }
    return timestamp;
}

void Epoll::fillActiveChannels(int numEvents, ChannelList *activeChannels) const
{
    assert(numEvents <= int(m_events.size()));
    for (int i = 0; i < numEvents; ++i)
    {
        Channel* channel = static_cast<Channel*>(m_events[i].data.ptr);
#ifndef NDEBUG
        int fd = channel->fd();
        ChannelMap::const_iterator it = channelMap.find(fd);
        assert(it != channelMap.end());
        assert(it->second == channel);
#endif
        channel->setRevents(m_events[i].events);
        activeChannels->push_back(channel);
    }
}
