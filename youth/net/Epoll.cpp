#include <sys/epoll.h>
#include <unistd.h>
#include <assert.h>

#include "EventLoop.h"
#include "Epoll.h"
#include "Socket.h"
#include "Channel.h"

#include <youth/utils/LogOut.h>

namespace youth
{

using namespace utils;

namespace net
{

const int kNew = -1;
const int kAdded = 1;
const int kDeleted = 2;

Epoll::Epoll(EventLoop *eventLoop)
    : m_eventLoop(eventLoop)
    , m_eventVec(kInitEventListSize)
    , m_epollfd(::epoll_create(EPOLL_CLOEXEC))
{
    if (m_epollfd < 0)
    {
        LOG_FATAL << "EPoll";
    }
}

Epoll::~Epoll()
{
    ::close(m_epollfd);
}

Timestamp Epoll::poll(int timeoutMs, ChannelList *activeChannels)
{
    int numEvents = ::epoll_wait(m_epollfd, &*m_eventVec.begin(),
                                 static_cast<int>(m_eventVec.size()),
                                 timeoutMs);
    int savedErrno = errno;
    Timestamp timestamp(Timestamp::currentTimestamp());
    if (numEvents > 0)
    {
        LOG_DEBUG << numEvents << " events happened";
        fillActiveChannels(numEvents, activeChannels);
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
            LOG_ERROR << "EPoll::poll()";
        }
    }
    return timestamp;
}

void Epoll::updateChannel(Channel *channel)
{
    m_eventLoop->assertInLoopThread();
    const int index = channel->index();
    LOG_DEBUG << "Channel fd = " << channel->fd() << " events = " << channel->events() << " index = " << index;
    if (index == kNew || index == kDeleted)
    {
        // a new one, add with EPOLL_CTL_ADD
        int fd = channel->fd();
        if (index == kNew)
        {
            assert(m_channelMap.find(fd) == m_channelMap.end());
            m_channelMap[fd] = channel;
        }
        else // index == kDeleted
        {
            assert(m_channelMap.find(fd) != m_channelMap.end());
            assert(m_channelMap[fd] == channel);
        }

        channel->setIndex(kAdded);
        update(EPOLL_CTL_ADD, channel);
    }
    else
    {
        // update existing one with EPOLL_CTL_MOD/DEL
        int fd = channel->fd();
        (void)fd;
        assert(m_channelMap.find(fd) != m_channelMap.end());
        assert(m_channelMap[fd] == channel);
        assert(index == kAdded);
        if (channel->isNoneEvent())
        {
            update(EPOLL_CTL_DEL, channel);
            channel->setIndex(kDeleted);
        }
        else
        {
            update(EPOLL_CTL_MOD, channel);
        }
    }
}

void Epoll::removeChannel(Channel *channel)
{
    m_eventLoop->assertInLoopThread();
    int fd = channel->fd();
    LOG_DEBUG << "fd = " << fd;
    assert(m_channelMap.find(fd) != m_channelMap.end());
    assert(m_channelMap[fd] == channel);
    assert(channel->isNoneEvent());
    int index = channel->index();
    assert(index == kAdded || index == kDeleted);
    size_t n = m_channelMap.erase(fd);
    (void)n;
    assert(n == 1);

    if (index == kAdded)
    {
        update(EPOLL_CTL_DEL, channel);
    }
    channel->setIndex(kNew);
}

bool Epoll::hasChannel(Channel *channel) const
{
    m_eventLoop->assertInLoopThread();
    ChannelMap::const_iterator it = m_channelMap.find(channel->fd());
    return it != m_channelMap.end() && it->second == channel;
}

void Epoll::fillActiveChannels(int numEvents, ChannelList *activeChannels) const
{
    assert(numEvents <= int(m_eventVec.size()));
    for (int i = 0; i < numEvents; ++i)
    {
        Channel *channel = static_cast<Channel *>(m_eventVec[i].data.ptr);
#ifndef NDEBUG
        int fd = channel->fd();
        ChannelMap::const_iterator it = m_channelMap.find(fd);
        assert(it != m_channelMap.end());
        assert(it->second == channel);
#endif
        channel->setRevents(m_eventVec[i].events);
        activeChannels->push_back(channel);
    }
}

inline const char *operationToString(int op)
{
    switch (op)
    {
    case EPOLL_CTL_ADD:
        return "ADD";
    case EPOLL_CTL_DEL:
        return "DEL";
    case EPOLL_CTL_MOD:
        return "MOD";
    default:
        assert(false && "ERROR op");
        return "Unknown Operation";
    }
}

void Epoll::update(int operation, Channel *channel)
{
    struct epoll_event event;
    memset(&event, 0, sizeof event);
    event.events = channel->events();
    event.data.ptr = channel;
    int fd = channel->fd();
    LOG_DEBUG << "epoll_ctl op = " << operationToString(operation)
              << " fd = " << fd << " event = { " << channel->eventsToString() << " }";
    if (::epoll_ctl(m_epollfd, operation, fd, &event) < 0)
    {
        if (operation == EPOLL_CTL_DEL)
        {
            LOG_ERROR << "epoll_ctl op =" << operationToString(operation) << " fd =" << fd;
        }
        else
        {
            LOG_FATAL << "epoll_ctl op =" << operationToString(operation) << " fd =" << fd;
        }
    }
}

}

}
