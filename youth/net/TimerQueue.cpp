#include "TimerQueue.h"
#include "TimerId.h"
#include "Timer.h"
#include "EventLoop.h"
#include "Channel.h"

#include <youth/utils/Logging.h>

#include <sys/timerfd.h>
#include <unistd.h>
#include <assert.h>

// 利用timerfd创建定时器文件描述符，利用epoll等IO复用函数
// 通过监控文件描述符，时间到期，触发可读事件（回调函数）

namespace youth
{

using namespace utils;

namespace net
{

//创建定时器文件描述符
int createTimerfd()
{
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC,
                                   TFD_NONBLOCK | TFD_CLOEXEC);
    if (timerfd < 0)
    {
        LOG_FATAL << "Failed in timerfd_create";
    }
    return timerfd;
}

//定时 间隔
struct timespec howMuchTimeFromNow(Timestamp when)
{
    int64_t microseconds = when.microSecondsSinceEpoch()
            - Timestamp::currentTimestamp().microSecondsSinceEpoch();
    if (microseconds < 100)
    {
        microseconds = 100;
    }
    struct timespec ts;
    ts.tv_sec = time_t(microseconds / Timestamp::kMicroSecondsPerSecond);
    ts.tv_nsec = long((microseconds % Timestamp::kMicroSecondsPerSecond) * 1000);
    return ts;
}

//超时次数
void readTimerfd(int timerfd, Timestamp now)
{
    uint64_t howmany;
    ssize_t n = ::read(timerfd, &howmany, sizeof howmany);
    LOG_DEBUG << "TimerQueue::handleRead() " << howmany << " at " << now.microSecondsToString();
    if (n != sizeof howmany)
    {
        LOG_ERROR << "TimerQueue::handleRead() reads " << n << " bytes instead of 8";
    }
}

//设置定时器
void resetTimerfd(int timerfd, Timestamp expiration)
{
    // wake up loop by timerfd_settime()
    struct itimerspec newValue;
    struct itimerspec oldValue;
    memset(&newValue, 0, sizeof newValue);
    memset(&oldValue, 0, sizeof oldValue);
    newValue.it_value = howMuchTimeFromNow(expiration);
    int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
    if (ret)
    {
        LOG_ERROR << "timerfd_settime()";
    }
}

TimerQueue::TimerQueue(EventLoop *loop)
    : m_eventLoop(loop)
    , m_timerfd(createTimerfd())
    , m_timerfdChannelPtr(new Channel(loop, m_timerfd))
    , m_timers()
    , m_callingExpiredTimers(false)
{
    m_timerfdChannelPtr->setReadCallback(std::bind(&TimerQueue::handleRead, this, std::placeholders::_1));
    // we are always reading the timerfd, we disarm it with timerfd_settime.
    m_timerfdChannelPtr->enableReading();
}

TimerQueue::~TimerQueue()
{
    m_timerfdChannelPtr->disableAll();
    m_timerfdChannelPtr->remove();
    ::close(m_timerfd);
    // do not remove channel, since we're in EventLoop::dtor();
    for (const Entry& timer : m_timers)
    {
        delete timer.second;
    }
}

TimerId TimerQueue::addTimer(TimerCallback cb, Timestamp when, double interval)
{
    Timer* timer = new Timer(std::move(cb), when, interval);
    m_eventLoop->runInLoop(
                std::bind(&TimerQueue::addTimerInLoop, this, timer));
    return TimerId(timer, timer->sequence());
}

void TimerQueue::cancel(TimerId timerId)
{
    m_eventLoop->runInLoop(
                std::bind(&TimerQueue::cancelInLoop, this, timerId));
}

void TimerQueue::addTimerInLoop(Timer *timer)
{
    m_eventLoop->assertInLoopThread();
    bool earliestChanged = insert(timer);

    if (earliestChanged)
    {
        resetTimerfd(m_timerfd, timer->expiration());
    }
}

void TimerQueue::cancelInLoop(TimerId timerId)
{
    m_eventLoop->assertInLoopThread();
    assert(m_timers.size() == m_activeTimers.size());
    ActiveTimer timer(timerId.m_timer, timerId.m_sequence);
    ActiveTimerSet::iterator it = m_activeTimers.find(timer);
    if(it != m_activeTimers.end())
    {
        size_t n = m_timers.erase(Entry(it->first->expiration(), it->first));
        assert(n == 1);
        (void)n;
        delete it->first; // FIXME: no delete please
        m_activeTimers.erase(it);
    }
    else if(m_callingExpiredTimers)
    {
        m_cancelingTimers.insert(timer);
    }
    assert(m_timers.size() == m_activeTimers.size());
}

void TimerQueue::handleRead(Timestamp now)
{
    m_eventLoop->assertInLoopThread();
    //Timestamp now(Timestamp::currentTimestamp());
    readTimerfd(m_timerfd, now);

    std::vector<Entry> expired = getExpired(now);

    m_callingExpiredTimers = true;
    m_cancelingTimers.clear();
    // safe to callback outside critical section
    for (const Entry& it : expired)
    {
        it.second->run();
    }
    m_callingExpiredTimers = false;

    reset(expired, now);
}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now)
{
    assert(m_timers.size() == m_activeTimers.size());
    std::vector<Entry> expired;
    Entry sentry(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
    TimerList::iterator end = m_timers.lower_bound(sentry);
    assert(end == m_timers.end() || now < end->first);
    std::copy(m_timers.begin(), end, back_inserter(expired));
    m_timers.erase(m_timers.begin(), end);

    for (const Entry& it : expired)
    {
        ActiveTimer timer(it.second, it.second->sequence());
        size_t n = m_activeTimers.erase(timer);
        assert(n == 1);
        (void)n;
    }

    assert(m_timers.size() == m_activeTimers.size());
    return expired;
}

void TimerQueue::reset(const std::vector<TimerQueue::Entry> &expired, Timestamp now)
{
    Timestamp nextExpire;

    for (const Entry& it : expired)
    {
        ActiveTimer timer(it.second, it.second->sequence());
        // 重复定时触发 now存在误差，会损失精度
        if (it.second->repeat()
                && m_cancelingTimers.find(timer) == m_cancelingTimers.end())
        {
            it.second->restart(now);
            insert(it.second);
        }
        else
        {
            // FIXME move to a free list
            delete it.second; // FIXME: no delete please
        }
    }

    if (!m_timers.empty())
    {
        nextExpire = m_timers.begin()->second->expiration();
    }

    if (nextExpire.isValid())
    {
        resetTimerfd(m_timerfd, nextExpire);
    }
}

bool TimerQueue::insert(Timer *timer)
{
    m_eventLoop->assertInLoopThread();
    assert(m_timers.size() == m_activeTimers.size());
    bool earliestChanged = false;
    Timestamp when = timer->expiration();
    TimerList::iterator it = m_timers.begin();
    if (it == m_timers.end() || when < it->first)
    {
        earliestChanged = true;
    }
    {
        std::pair<TimerList::iterator, bool> result
                = m_timers.insert(Entry(when, timer));
        assert(result.second);
        (void)result;
    }
    {
        std::pair<ActiveTimerSet::iterator, bool> result
                = m_activeTimers.insert(ActiveTimer(timer, timer->sequence()));
        assert(result.second);
        (void)result;
    }

    assert(m_timers.size() == m_activeTimers.size());
    return earliestChanged;
}

}

}

