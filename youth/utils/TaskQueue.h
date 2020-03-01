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
        :mutex()
        ,queue(){
    }

    void append(const T& t){
        MutexLock lock(mutex);
        queue.push_back(t);
    }

    void append(const T&& t){
        MutexLock lock(mutex);
        queue.push_back(std::move(t));
    }

    T take(){
        MutexLock lock(mutex);
        if(queue.empty())
            return T();
        T t(std::move(queue.front()));
        queue.pop_front();
        return t;
    }

    size_t size() const{
        MutexLock lock(mutex);
        return queue.size();
    }

private:
    mutable Mutex mutex;
    std::deque<T> queue GUARDED_BY(mutex);
};

}

#endif // TASKQUEUE_H
