#ifndef TIMERQUEUE_H
#define TIMERQUEUE_H

#include <youth/core/Timestamp.h>

#include "Callbacks.h"

#include <set>

namespace youth
{

using namespace core;

namespace net
{

class Channel;
class EventLoop;
class Timer;
class TimerId;
class TimerQueue : noncopyable
{
public:
    explicit TimerQueue(EventLoop* loop);
    ~TimerQueue();

    ///
    /// Schedules the callback to be run at given time,
    /// repeats if @c interval > 0.0.
    ///
    /// Must be thread safe. Usually be called from other threads.
    TimerId addTimer(TimerCallback cb,
                     Timestamp when,
                     double interval);

    void cancel(TimerId timerId);

private:
    // FIXME: use unique_ptr<Timer> instead of raw pointers.
    // This requires heterogeneous comparison lookup (N3465) from C++14
    // so that we can find an T* in a set<unique_ptr<T>>.
    typedef std::pair<Timestamp, Timer*> Entry;
    typedef std::set<Entry> TimerList;
    typedef std::pair<Timer*, int64_t> ActiveTimer;
    typedef std::set<ActiveTimer> ActiveTimerSet;

    void addTimerInLoop(Timer* timer);
    void cancelInLoop(TimerId timerId);
    // called when timerfd alarms
    void handleRead(Timestamp now);
    // move out all expired timers
    std::vector<Entry> getExpired(Timestamp now);
    void reset(const std::vector<Entry>& expired, Timestamp now);

    bool insert(Timer* timer);

    EventLoop* m_eventLoop;
    const int m_timerfd;
    std::unique_ptr<Channel> m_timerfdChannelPtr;
    // Timer list sorted by expiration
    TimerList m_timers;

    // for cancel()
    ActiveTimerSet m_activeTimers;
    bool m_callingExpiredTimers; /* atomic */
    ActiveTimerSet m_cancelingTimers;
};

}

}

#endif // TIMERQUEUE_H
