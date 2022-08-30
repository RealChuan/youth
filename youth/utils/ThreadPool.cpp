#include "ThreadPool.h"

#include <youth/core/CountDownLatch.h>

#include <assert.h>
#include <unistd.h>

namespace youth {

namespace utils {

ThreadPool::ThreadPool(const std::string &name)
    : m_name(name)
    , m_maxTaskNum(0)
    , m_running(false)
    , m_mutex()
    , m_notEmpty()
    , m_notFull()
{}

ThreadPool::~ThreadPool()
{
    if (m_running) {
        stop();
    }
}

void ThreadPool::start(int numThreads)
{
    assert(m_threadVec.empty());
    m_running = true;
    m_threadVec.reserve(static_cast<unsigned long>(numThreads));
    for (int i = 0; i < numThreads; ++i) {
        std::string name = m_name + std::to_string(i + 1);
        auto thread = new Thread(std::bind(&ThreadPool::threadFunc, this), name);
        m_threadVec.emplace_back(thread);
        thread->start();
    }
    if (numThreads == 0 && m_threadInitCallback) {
        m_threadInitCallback();
    }
}

void ThreadPool::stop()
{
    {
        MutexLock lock(m_mutex);
        m_running = false;
        m_notEmpty.notify_all();
        m_notFull.notify_all();
    }
    for (auto &thr : m_threadVec) {
        thr->join();
    }
    m_queue.clear();
}

void ThreadPool::waitForAllDone()
{
    CountDownLatch latch(1);
    run([&] { latch.countDown(); });
    latch.wait();
}

void ThreadPool::threadFunc()
{
    assert(m_running == true);

    if (m_threadInitCallback) {
        m_threadInitCallback();
    }
    while (m_running) {
        Task task(take());
        if (task) {
            task();
        }
    }
}

void ThreadPool::run(Task task)
{
    if (m_threadVec.empty()) {
        task();
    } else {
        MutexLock lock(m_mutex);
        while (isFull()) {
            m_notFull.wait(lock);
        }
        assert(!isFull());
        m_queue.push_back(std::move(task));
        m_notEmpty.notify_one();
    }
}

long ThreadPool::cpuCores()
{
    //sysconf(_SC_NPROCESSORS_ONLN);
    return sysconf(_SC_NPROCESSORS_CONF);
}

ThreadPool::Task ThreadPool::take()
{
    MutexLock lock(m_mutex);
    // always use a while-loop, due to spurious wakeup
    while (m_queue.empty() && m_running) {
        m_notEmpty.wait(lock);
    }
    Task task;
    if (m_queue.empty()) {
        return task;
    }
    task = m_queue.front();
    m_queue.pop_front();
    if (m_maxTaskNum > 0) {
        m_notFull.notify_one();
    }
    return task;
}

} // namespace utils

} // namespace youth
