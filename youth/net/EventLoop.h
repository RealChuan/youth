#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "youth/core/Object.h"
#include "youth/utils/Thread.h"
#include "youth/core/Timestamp.h"

#include <sys/types.h>
#include <vector>
#include <memory>

namespace youth
{

class Channel;
class Epoll;
class EventLoop : public noncopyable
{
    typedef std::function<void()> Functor;

public:
    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    /// Runs callback immediately in the loop thread.
    /// It wakes up the loop, and run the cb.
    /// If in the same loop thread, cb is run within the function.
    /// Safe to call from other threads.
    void runInLoop(Functor cb);
    /// Queues callback in the loop thread.
    /// Runs after finish pooling.
    /// Safe to call from other threads.
    void queueInLoop(Functor cb);

    size_t queueSize() const;

    // internal usage
    void wakeup();
    void updateChannel(Channel *channel);
    void removeChannel(Channel *channel);
    bool hasChannel(Channel *channel);

    bool isInLoopThread() const;

private:
    void abortNotInLoopThread();
    void assertInLoopThread();

    void doPendingFunctors();

    void printActiveChannels() const; // DEBUG

    typedef std::vector<Channel *> ChannelList;

    const pid_t m_threadID;
    volatile bool m_looping; /* atomic */
    volatile bool m_quit;

    volatile bool m_callingPendingFunctors; /* atomic */
    volatile bool m_eventHandling;          /* atomic */
    int64_t m_index;
    Timestamp m_pollReturnTime;
    std::unique_ptr<Epoll> m_epoll;

    int m_wakeupFd;

    // scratch variables
    ChannelList m_activeChannels;
    Channel *m_currentActiveChannel;

    mutable Mutex m_mutex;
    std::vector<Functor> m_pendingFunctors GUARDED_BY(m_mutex);
};

} // namespace youth

#endif // EVENTLOOP_H
