#include "Channel.h"
#include "EventLoop.h"

#include <youth/utils/Logging.h>

#include <assert.h>
#include <sstream>
#include <sys/epoll.h>
#include <poll.h>

namespace youth
{

using namespace utils;

namespace net
{

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT;

Channel::Channel(EventLoop *loop, int fd)
    : m_eventLoop(loop)
    , m_index(-1) //epoll knew
    , m_fd(fd)
    , m_events(0)
    , m_revents(0)
    , m_logHup(true)
    , m_eventHandling(false)
    , m_addedToLoop(false)
    , m_tied(false)
{
}

Channel::~Channel()
{
    assert(!m_eventHandling);
    assert(!m_addedToLoop);
    if (m_eventLoop->isInLoopThread())
    {
        assert(!m_eventLoop->hasChannel(this));
    }
}

void Channel::handleEvent(Timestamp receiveTime)
{
    std::shared_ptr<void> guard;
    if (m_tied)
    {
        guard = m_tie.lock();
        if (guard)
        {
            handleEventWithGuard(receiveTime);
        }
    }
    else
    {
        handleEventWithGuard(receiveTime);
    }
}

void Channel::setReadCallback(ReadEventCallback cb)
{
    m_readCallback = std::move(cb);
}

void Channel::setWriteCallback(Channel::EventCallback cb)
{
    m_writeCallback = std::move(cb);
}

void Channel::setCloseCallback(Channel::EventCallback cb)
{
    m_closeCallback = std::move(cb);
}

void Channel::setErrorCallback(Channel::EventCallback cb)
{
    m_errorCallback = std::move(cb);
}

void Channel::tie(const std::shared_ptr<void> &obj)
{
    m_tie = obj;
    m_tied = true;
}

void Channel::enableReading()
{
    m_events |= kReadEvent;
    update();
}

void Channel::disableReading()
{
    m_events &= ~kReadEvent;
    update();
}

void Channel::enableWriting()
{
    m_events |= kWriteEvent;
    update();
}

void Channel::disableWriting()
{
    m_events &= ~kWriteEvent;
    update();
}

void Channel::disableAll()
{
    m_events = kNoneEvent;
    update();
}

void Channel::update()
{
    m_addedToLoop = true;
    m_eventLoop->updateChannel(this);
}

void Channel::remove()
{
    assert(isNoneEvent());
    m_addedToLoop = false;
    m_eventLoop->removeChannel(this);
}

bool Channel::isWriting() const
{
    return m_events & kWriteEvent;
}

bool Channel::isReading() const
{
    return m_events & kReadEvent;
}

bool Channel::isNoneEvent() const
{
    return m_events == kNoneEvent;
}

int Channel::index() const
{
    return m_index;
}

void Channel::setIndex(int idx)
{
    m_index = idx;
}

EventLoop *Channel::ownerLoop() const
{
    return m_eventLoop;
}

std::string Channel::eventsToString() const
{
    return eventsToString(m_fd, m_events);
}

std::string Channel::reventsToString() const
{
    return eventsToString(m_fd, m_revents);
}

int Channel::fd() const
{
    return m_fd;
}

void Channel::setRevents(int revents)
{
    m_revents = revents;
}

int Channel::events() const
{
    return m_events;
}

void Channel::handleEventWithGuard(Timestamp receiveTime)
{
    m_eventHandling = true;
    LOG_DEBUG << reventsToString();
    if ((m_revents & EPOLLHUP) && !(m_revents & EPOLLIN))
    {
        if (m_logHup)
        {
            LOG_WARN << "fd = " << m_fd << " Channel::handle_event() POLLHUP";
        }
        if (m_closeCallback)
            m_closeCallback();
    }

    if (m_revents & POLLNVAL)
    {
        LOG_WARN << "fd = " << m_fd << " Channel::handle_event() POLLNVAL";
    }

    if (m_revents & (EPOLLERR | POLLNVAL))
    {
        if (m_errorCallback)
            m_errorCallback();
    }
    if (m_revents & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
    {
        if (m_readCallback)
            m_readCallback(receiveTime);
    }
    if (m_revents & EPOLLOUT)
    {
        if (m_writeCallback)
            m_writeCallback();
    }
    m_eventHandling = false;
}

std::string Channel::eventsToString(int fd, int events)
{
    std::ostringstream oss;
    oss << fd << ": ";
    if (events & EPOLLIN)
        oss << "IN ";
    if (events & EPOLLPRI)
        oss << "PRI ";
    if (events & EPOLLOUT)
        oss << "OUT ";
    if (events & EPOLLHUP)
        oss << "HUP ";
    if (events & EPOLLRDHUP)
        oss << "RDHUP ";
    if (events & EPOLLERR)
        oss << "ERR ";
    if (events & POLLNVAL)
        oss << "NVAL ";

    return oss.str();
}

}

}
