#ifndef THREAD_H
#define THREAD_H

#include <youth/core/Object.h>

#include <functional>
#include <string>

namespace youth
{

using namespace core;

namespace utils
{

class Thread : noncopyable
{
    typedef std::function<void()> ThreadFunc;
public:
    Thread(ThreadFunc func_);
    ~Thread();

    void start();
    int join(); // return pthread_join()
    bool isRunning() const;
    pthread_t pthreadID() const;

    static void* threadFunc(void *obj)
    {
        Thread *pThis = static_cast<Thread *>(obj);
        pThis->m_func(); //------
        return 0;
    }

private:	
    bool m_running;
    bool m_joined;
    pthread_t m_pthreadId;
    //pid_t _tid;
    ThreadFunc m_func;
    //std::string _name;
};

}

}

#endif /* THREAD_H */
