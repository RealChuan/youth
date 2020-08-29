#ifndef THREAD_H
#define THREAD_H

#include <youth/core/Object.h>

#include <functional>
#include <string>
#include <atomic>
#include <memory>

namespace youth
{

using namespace core;

namespace utils
{

class Thread : noncopyable
{
    typedef std::function<void()> ThreadFunc;
public:
    Thread(ThreadFunc threadFunc, const std::string &name = std::string());
    ~Thread();

    void start();
    int join(); // pthread_join()
    int cancel(); // pthread_cancel()

    bool isRunning() const{ return m_running; }

    pthread_t pthreadID() const{ return m_pthreadId; }
    pid_t tid() const { return m_tid; }
    std::string name() const { return m_name; }

    static void* threadFunc(void *obj);
    static int64_t numCreated() { return m_numCreated; }
    static int64_t allThreadNum() { return m_allThreadNum; }

private:
    void setDefaultName();

    bool m_running = false;
    bool m_joined = false;
    pthread_t m_pthreadId;
    // pthread_attr_t m_attr;
    pid_t m_tid;
    ThreadFunc m_func;
    std::string m_name;

    static std::atomic<int64_t> m_numCreated;
    static std::atomic<int64_t> m_allThreadNum;
};

}

}

#endif /* THREAD_H */
