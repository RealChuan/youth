#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h"
#include "Socket.h"
#include "SocketFunc.h"
#include "TimerQueue.h"

#include <youth/utils/Logging.h>
#include <youth/core/CurrentThread.h>

#include <assert.h>
#include <sys/eventfd.h>

namespace youth
{

using namespace utils;

namespace net
{

__thread EventLoop *g_loopInThisThread = nullptr;

const int kPollTimeMs = 10000;

int createEventfd()
{
    int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (evtfd < 0)
    {
        LOG_ERROR << "Failed in eventfd";
        abort();
    }
    return evtfd;
}

EventLoop::EventLoop()
    : m_threadID(CurrentThread::tid())
    , m_looping(false)
    , m_quit(false)
    , m_callingPendingFunctors(false)
    , m_eventHandling(false)
    , m_index(0)
    , m_epoll(new Epoll(this))
    , m_timerQueuePtr(new TimerQueue(this))
    , m_wakeupFd(createEventfd())
    , m_wakeupChannelPtr(new Channel(this, m_wakeupFd))
{
    LOG_DEBUG << "EventLoop created " << this << " in thread " << m_threadID;
    if (g_loopInThisThread)
    {
        LOG_FATAL << "Another EventLoop " << g_loopInThisThread
                  << " exists in this thread " << m_threadID;
    }
    else
    {
        g_loopInThisThread = this;
    }
    m_wakeupChannelPtr->setReadCallback(std::bind(&EventLoop::handleRead, this));
    m_wakeupChannelPtr->enableReading();
}

EventLoop::~EventLoop()
{
    LOG_DEBUG << "EventLoop " << this << " of thread " << m_threadID
              << " destructs in thread " << CurrentThread::tid();
    m_wakeupChannelPtr->disableAll();
    m_wakeupChannelPtr->remove();
    ::close(m_wakeupFd);
    g_loopInThisThread = nullptr;
}

void EventLoop::loop()
{
    assert(!m_looping);
    assertInLoopThread();
    m_looping = true;
    m_quit = false; // FIXME: what if someone calls quit() before loop() ?
    LOG_DEBUG << "EventLoop " << this << " start looping";
    while (!m_quit)
    {
        m_activeChannels.clear();
        m_pollReturnTime = m_epoll->poll(kPollTimeMs, &m_activeChannels);
        m_index++;
        if (Logging::getLogLevel() <= Logging::DEBUG)
        {
            printActiveChannels();
        }
        // TODO sort channel by priority
        m_eventHandling = true;
        for (Channel *channel : m_activeChannels)
        {
            m_currentActiveChannel = channel;
            m_currentActiveChannel->handleEvent(m_pollReturnTime);
        }
        m_currentActiveChannel = NULL;
        m_eventHandling = false;
        doPendingFunctors();
        // Todo loop
    }

    LOG_DEBUG << "EventLoop " << this << " stop looping";
    m_looping = false;
}

void EventLoop::quit()
{
    m_quit = true;
}

void EventLoop::runInLoop(Functor cb)
{
    if (isInLoopThread())
    {
        cb();
    }
    else
    {
        queueInLoop(std::move(cb));
    }
}

void EventLoop::queueInLoop(Functor cb)
{
    {
        MutexLock lock(m_mutex);
        m_pendingFunctors.push_back(std::move(cb));
    }
    if (!isInLoopThread() || m_callingPendingFunctors)
    {
        wakeup();
    }
}

size_t EventLoop::queueSize() const
{
    MutexLock lock(m_mutex);
    return m_pendingFunctors.size();
}

TimerId EventLoop::runAt(Timestamp time, TimerCallback cb)
{
    return m_timerQueuePtr->addTimer(std::move(cb), time, 0.0);
}

TimerId EventLoop::runAfter(double delay, TimerCallback cb)
{
    Timestamp time(addTime(Timestamp::currentTimestamp(), delay));
    return runAt(time, std::move(cb));
}

TimerId EventLoop::runEvery(double interval, TimerCallback cb)
{
    Timestamp time(addTime(Timestamp::currentTimestamp(), interval));
    return m_timerQueuePtr->addTimer(std::move(cb), time, interval);
}

void EventLoop::cancel(TimerId timerId)
{
    return m_timerQueuePtr->cancel(timerId);
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    ssize_t n = SocketFunc::write(m_wakeupFd, &one, sizeof one);
    if (n != sizeof one)
    {
        LOG_ERROR << "EventLoop::wakeup() writes " << n << " bytes instead of 8";
    }
}

void EventLoop::handleRead()
{
    uint64_t one = 1;
    ssize_t n = SocketFunc::read(m_wakeupFd, &one, sizeof one);
    if (n != sizeof one)
    {
        LOG_ERROR << "EventLoop::handleRead() reads " << n << " bytes instead of 8";
    }
}

void EventLoop::updateChannel(Channel *channel)
{
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    m_epoll->updateChannel(channel);
}

void EventLoop::removeChannel(Channel *channel)
{
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    if (m_eventHandling)
    {
        assert(m_currentActiveChannel == channel ||
               std::find(m_activeChannels.begin(), m_activeChannels.end(), channel) == m_activeChannels.end());
    }
    m_epoll->removeChannel(channel);
}

bool EventLoop::hasChannel(Channel *channel)
{
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    return m_epoll->hasChannel(channel);
}

bool EventLoop::isInLoopThread() const
{
    return m_threadID == CurrentThread::tid();
}

void EventLoop::abortNotInLoopThread()
{
    LOG_FATAL << "EventLoop::abortNotInLoopThread - EventLoop " << this
              << " was created in thread id = " << m_threadID
              << ", current thread id = " << CurrentThread::tid();
}

void EventLoop::assertInLoopThread()
{
    if (!isInLoopThread())
    {
        abortNotInLoopThread();
    }
}

bool EventLoop::eventHandling() const
{
    return m_eventHandling;
}

void EventLoop::setContext(const std::any &context)
{
    m_context = context;
}

const std::any &EventLoop::getContext() const
{
    return m_context;
}

std::any *EventLoop::getMutableContext()
{
    return &m_context;
}

void EventLoop::doPendingFunctors()
{
    std::vector<Functor> functors;
    m_callingPendingFunctors = true;
    {
        MutexLock lock(m_mutex);
        functors.swap(m_pendingFunctors);
    }
    for (const Functor &functor : functors)
    {
        functor();
    }
    m_callingPendingFunctors = false;
}

void EventLoop::printActiveChannels() const
{
    for (const Channel *channel : m_activeChannels)
    {
        LOG_DEBUG << "{" << channel->reventsToString() << "} ";
    }
}

}

}
