#include "EventLoopThread.h"
#include "EventLoop.h"

#include <assert.h>

namespace youth {

namespace net {

EventLoopThread::EventLoopThread(const EventLoopThread::ThreadInitCallback &cb,
                                 const std::string &name)
    : m_exit(false)
    , m_mutex()
    , m_cond()
    , m_loop(nullptr)
    , m_thread(std::bind(&EventLoopThread::threadFunc, this), name)
    , m_callback(cb)
{}

EventLoopThread::~EventLoopThread()
{
    m_exit = true;
    if (m_loop != nullptr) {
        m_loop->quit();
        m_thread.join();
    }
}

EventLoop *EventLoopThread::startLoop()
{
    assert(!m_thread.isRunning());
    m_thread.start();

    EventLoop *loop = NULL;
    {
        MutexLock lock(m_mutex);
        while (m_loop == NULL) {
            m_cond.wait(lock);
        }
        loop = m_loop;
    }

    return loop;
}

void EventLoopThread::threadFunc()
{
    EventLoop loop;
    if (m_callback) {
        m_callback(&loop);
    }

    {
        MutexLock lock(m_mutex);
        m_loop = &loop;
        m_cond.notify_one();
    }

    loop.loop();
    MutexLock lock(m_mutex);
    m_loop = nullptr;
}

} // namespace net

} // namespace youth
