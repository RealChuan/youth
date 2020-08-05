#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include "Thread.h"

#include <deque>

namespace youth {

template<typename T>
class TaskQueue
{
public:
    TaskQueue()
        :m_mutex()
        ,m_queue(){
    }

    void append(const T& t){
        MutexLock lock(m_mutex);
        m_queue.push_back(t);
    }

    void append(const T&& t){
        MutexLock lock(m_mutex);
        m_queue.push_back(std::move(t));
    }

    T take(){
        MutexLock lock(m_mutex);
        if(m_queue.empty())
            return T();
        T t(std::move(m_queue.front()));
        m_queue.pop_front();
        return t;
    }

    size_t size() const{
        MutexLock lock(m_mutex);
        return m_queue.size();
    }

private:
    mutable Mutex m_mutex;
    std::deque<T> m_queue GUARDED_BY(m_mutex);
};

}

#endif // TASKQUEUE_H
