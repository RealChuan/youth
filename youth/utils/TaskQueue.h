#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include <youth/core/Thread.hpp>

#include <deque>

namespace youth {

using namespace core;

namespace utils {

template<typename T>
class TaskQueue : copyable
{
public:
    TaskQueue()
        : m_mutex()
        , m_queue()
    {}

    void append(const T &t)
    {
        MutexLock lock(m_mutex);
        m_queue.push_back(t);
    }

    void append(const T &&t)
    {
        MutexLock lock(m_mutex);
        m_queue.push_back(std::move(t));
    }

    T take()
    {
        MutexLock lock(m_mutex);
        if (m_queue.empty())
            return T();
        T t(std::move(m_queue.front()));
        m_queue.pop_front();
        return t;
    }

    size_t size() const
    {
        MutexLock lock(m_mutex);
        return m_queue.size();
    }

private:
    mutable Mutex m_mutex;
    std::deque<T> m_queue;
};

} // namespace utils

} // namespace youth

#endif // TASKQUEUE_H
