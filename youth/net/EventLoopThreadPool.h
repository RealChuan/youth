#ifndef EVENTLOOPTHREADPOOL_H
#define EVENTLOOPTHREADPOOL_H

#include <youth/core/Object.h>

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace youth {

using namespace core;

namespace net {

class EventLoop;
class EventLoopThread;
class EventLoopThreadPool : noncopyable
{
public:
    using ThreadInitCallback = std::function<void(EventLoop *)>;

    EventLoopThreadPool(EventLoop *baseLoop, std::string_view name);
    ~EventLoopThreadPool();
    void setThreadNum(int numThreads) { m_numThreads = numThreads; }
    void start(const ThreadInitCallback &cb = ThreadInitCallback());

    // valid after calling start()
    /// round-robin
    EventLoop *getNextLoop();

    /// with the same hash code, it will always return the same EventLoop
    EventLoop *getLoopForHash(size_t hashCode);

    std::vector<EventLoop *> getAllLoops();

    bool started() const { return m_started; }

    const std::string &name() const { return m_name; }

private:
    EventLoop *m_baseLoop;
    std::string m_name;
    bool m_started;
    int m_numThreads;
    int m_next;
    std::vector<std::unique_ptr<EventLoopThread>> m_threadPtrs;
    std::vector<EventLoop *> m_loops;
};

} // namespace net

} // namespace youth

#endif // EVENTLOOPTHREADPOOL_H
