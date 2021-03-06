#ifndef EVENTLOOPTHREAD_H
#define EVENTLOOPTHREAD_H

#include <youth/utils/Thread.h>
#include <youth/core/Condition.h>

#include <functional>
#include <string>

namespace youth
{

using namespace core;
using namespace utils;

namespace net
{

class EventLoop;
class EventLoopThread : noncopyable
{
    typedef std::function<void(EventLoop*)> ThreadInitCallback;
public:
    EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback(),
                    const std::string& name = std::string());
    ~EventLoopThread();

    EventLoop* startLoop();

private:
    void threadFunc();

    bool m_exit;
    Mutex m_mutex;
    Condition m_cond GUARDED_BY(m_mutex);
    EventLoop* m_loop GUARDED_BY(m_mutex);
    Thread m_thread;
    ThreadInitCallback m_callback;
};

}

}

#endif // EVENTLOOPTHREAD_H
