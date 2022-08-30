#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <deque>
#include <memory>
#include <vector>

#include <youth/core/Thread.hpp>

namespace youth {

using namespace core;

namespace utils {

class ThreadPool : noncopyable
{
public:
    using Task = std::function<void()>;

    ThreadPool(const std::string &name = std::string());
    ~ThreadPool();

    void setTaskNum(int maxTaskNum) { m_maxTaskNum = maxTaskNum; }
    void setThreadInitCallback(const Task &cb) { m_threadInitCallback = cb; }

    void start(int numThreads);
    void stop();
    void waitForAllDone();

    size_t queueSize() const
    {
        MutexLock lock(m_mutex);
        return m_queue.size();
    }

    void run(Task f);

    static long cpuCores();

private:
    // must lock
    bool isFull() const { return m_maxTaskNum > 0 && m_queue.size() >= m_maxTaskNum; }

    void threadFunc();
    Task take();

    std::string m_name;
    Task m_threadInitCallback;
    size_t m_maxTaskNum;
    bool m_running;

    mutable Mutex m_mutex;
    Condition m_notEmpty;
    Condition m_notFull;

    std::vector<std::unique_ptr<Thread>> m_threadVec;
    std::deque<Task> m_queue;
};

} // namespace utils

} // namespace youth

#endif /* THREADPOOL_H */
