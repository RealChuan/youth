#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <deque>
#include <memory>
#include <functional>

#include "Thread.h"

#include <youth/core/Condition.h>

namespace youth
{

using namespace core;

namespace utils
{

class ThreadPool : noncopyable
{
    typedef std::function<void ()> Task;

public:
    ThreadPool();
    ~ThreadPool();

    void setTaskNum(int maxTaskNum) { m_maxTaskNum = maxTaskNum; }
    void setThreadInitCallback(const Task& cb)
    {  m_threadInitCallback = cb; }

    void start(int numThreads);
    void stop();

    size_t queueSize() const
    {
        MutexLock lock(m_mutex);
        return m_queue.size();
    }

    void run(Task f);

private:

    bool isFull() const REQUIRES(m_mutex)
    {
        MutexLock lock(m_mutex);
        return m_maxTaskNum > 0 && m_queue.size() >= m_maxTaskNum;
    }

    void threadFunc();
    Task take();

    Task m_threadInitCallback;
    size_t m_maxTaskNum;
    bool m_running;
    mutable Mutex m_mutex;

    Condition m_notEmpty GUARDED_BY(m_mutex);
    Condition m_notFull GUARDED_BY(m_mutex);

    std::vector<std::unique_ptr<Thread>> m_threadVec;
    std::deque<Task> m_queue GUARDED_BY(m_mutex);
};

}

}

#endif /* THREADPOOL_H */
