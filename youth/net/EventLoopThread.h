#ifndef EVENTLOOPTHREAD_H
#define EVENTLOOPTHREAD_H

#include <youth/core/Thread.hpp>

namespace youth {

using namespace core;

namespace net {

class EventLoop;
class EventLoopThread : noncopyable
{
public:
    using ThreadInitCallback = std::function<void(EventLoop *)>;

    EventLoopThread(const ThreadInitCallback &cb = ThreadInitCallback(),
                    std::string_view name = std::string_view());
    ~EventLoopThread();

    EventLoop *startLoop();

private:
    void threadFunc();

    bool m_exit;
    Mutex m_mutex;
    Condition m_cond;
    EventLoop *m_loop;
    Thread m_thread;
    ThreadInitCallback m_callback;
};

} // namespace net

} // namespace youth

#endif // EVENTLOOPTHREAD_H
