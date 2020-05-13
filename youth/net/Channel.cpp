#include "Channel.h"
#include "EventLoop.h"
#include "youth/utils/LogOut.h"

#include <assert.h>
#include <sstream>
#include <poll.h>

using namespace youth;

Channel::Channel(EventLoop *loop, int fd)
    : m_loop(loop)
    , m_fd(fd)
    , m_events(0)
    , m_revents(0)
    , m_logHup(true)
    , m_eventHandling(false)
{

}

Channel::~Channel()
{
    if(m_loop->isInLoopThread())
    {

    }
}

void Channel::handleEvent(Timestamp receiveTime)
{
    handleEventWithGuard(receiveTime);
}

void youth::Channel::setReadCallback(ReadEventCallback cb)
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

void Channel::handleEventWithGuard(Timestamp receiveTime)
{
    m_eventHandling = true;
    LOG_DEBUG << reventsToString();
    if ((m_revents & POLLHUP) && !(m_revents & POLLIN))
    {
        if (m_logHup)
        {
            LOG_WARN << "fd = " << m_fd << " Channel::handle_event() POLLHUP";
        }
        if (m_closeCallback) m_closeCallback();
    }

    if (m_revents & POLLNVAL)
    {
        LOG_WARN << "fd = " << m_fd << " Channel::handle_event() POLLNVAL";
    }

    if (m_revents & (POLLERR | POLLNVAL))
    {
        if (m_errorCallback) m_errorCallback();
    }
    if (m_revents & (POLLIN | POLLPRI | POLLRDHUP))
    {
        if (m_readCallback) m_readCallback(receiveTime);
    }
    if (m_revents & POLLOUT)
    {
        if (m_writeCallback) m_writeCallback();
    }
    m_eventHandling = false;
}

std::string Channel::eventsToString(int fd, int events)
{
    std::ostringstream oss;
    oss << fd << ": ";
    if (events & POLLIN) oss << "IN ";
    if (events & POLLPRI) oss << "PRI ";
    if (events & POLLOUT) oss << "OUT ";
    if (events & POLLHUP) oss << "HUP ";
    if (events & POLLRDHUP) oss << "RDHUP ";
    if (events & POLLERR) oss << "ERR ";
    if (events & POLLNVAL) oss << "NVAL ";

    return oss.str();
}

