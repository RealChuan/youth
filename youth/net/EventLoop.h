#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "youth/core/Object.h"

#include <sys/types.h>

namespace youth
{

class EventLoop : public noncopyable
{
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    bool isInLoopThread() const;

private:
    void abortNotInLoopThread();
    void assertInLoopThread();

    const pid_t m_threadID;
    volatile bool m_looping; /* atomic */
    volatile bool m_quit;
};

}

#endif // EVENTLOOP_H
