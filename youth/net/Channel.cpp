#include "Channel.h"
#include "EventLoop.h"

#include <youth/utils/Logging.h>

#include <assert.h>
#include <sstream>
#include <sys/epoll.h>
#include <poll.h>

// EPOLLIN ：表示对应的文件描述符可以读（包括对端SOCKET正常关闭）；
// EPOLLOUT：表示对应的文件描述符可以写；
// EPOLLPRI：表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
// EPOLLERR：表示对应的文件描述符发生错误；
// EPOLLHUP：表示对应的文件描述符被挂断；
// EPOLLET： 将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。
// EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里

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

void Channel::remove()
{
    assert(isNoneEvent());
    m_addedToLoop = false;
    m_eventLoop->removeChannel(this);
}

void Channel::update()
{
    m_addedToLoop = true;
    m_eventLoop->updateChannel(this);
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
