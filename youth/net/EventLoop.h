#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "TimerId.h"
#include "Callbacks.h"

#include <youth/utils/Thread.h>
#include <youth/core/Timestamp.h>
#include <youth/core/Mutex.h>

#include <sys/types.h>
#include <vector>
#include <memory>
#include <atomic>
#include <any>

namespace youth
{

namespace net
{

class TimerQueue;
class Channel;
class Epoll;
class EventLoop : noncopyable
{
    typedef std::function<void()> Functor;

public:
    EventLoop();
    ~EventLoop();

    void loop();
    void quit() { m_quit = true; }

    /// Runs callback immediately in the loop thread.
    /// It wakes up the loop, and run the cb.
    /// If in the same loop thread, cb is run within the function.
    /// Safe to call from other threads.
    void runInLoop(Functor cb);
    /// Queues callback in the loop thread.
    /// Runs after finish pooling.
    /// Safe to call from other threads.
    void queueInLoop(Functor cb);

    size_t queueSize() const
    {
        MutexLock lock(m_mutex);
        return m_pendingFunctors.size();
    }

    // timers

    ///
    /// Runs callback at 'time'.
    /// Safe to call from other threads.
    ///
    TimerId runAt(Timestamp time, TimerCallback cb);
    ///
    /// Runs callback after @c delay seconds.
    /// Safe to call from other threads.
    ///
    TimerId runAfter(double delay, TimerCallback cb);
    ///
    /// Runs callback every @c interval seconds.
    /// Safe to call from other threads.
    ///
    TimerId runEvery(double interval, TimerCallback cb);
    ///
    /// Cancels the timer.
    /// Safe to call from other threads.
    ///
    void cancel(TimerId timerId);

    // internal usage
    void wakeup();
    void updateChannel(Channel *channel);
    void removeChannel(Channel *channel);
    bool hasChannel(Channel *channel);

    bool isInLoopThread() const
    { return m_threadID == CurrentThread::tid(); }
    void assertInLoopThread();

    bool eventHandling() const
    { return m_eventHandling; }

    void setContext(const std::any& context) { m_context = context; }
    const std::any& getContext() const { return m_context; }
    std::any* getMutableContext() { return &m_context; }

private:
    void abortNotInLoopThread();
    void handleRead();  // waked up
    void doPendingFunctors();
    void printActiveChannels() const; // DEBUG

    const pid_t m_threadID;
    std::atomic<bool> m_looping; /* atomic */
    std::atomic<bool> m_quit;

    std::atomic<bool> m_callingPendingFunctors; /* atomic */
    std::atomic<bool> m_eventHandling;          /* atomic */
    int64_t m_index;
    Timestamp m_pollReturnTime;
    std::unique_ptr<Epoll> m_epoll;
    std::unique_ptr<TimerQueue> m_timerQueuePtr;

    int m_wakeupFd;

    std::unique_ptr<Channel> m_wakeupChannelPtr;
    std::any m_context;

    // scratch variables
    std::vector<Channel *> m_activeChannels;
    Channel *m_currentActiveChannel;

    mutable Mutex m_mutex;
    std::vector<Functor> m_pendingFunctors GUARDED_BY(m_mutex);
};

}

} // namespace youth

#endif // EVENTLOOP_H
