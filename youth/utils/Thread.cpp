#include "Thread.h"
#include "Logging.h"

#include <youth/core/CountDownLatch.h>
#include <youth/core/Timestamp.h>
#include <youth/core/CurrentThread.h>

#include <assert.h>

namespace youth
{

namespace utils
{

std::atomic<int64_t> Thread::m_numCreated = 0;
std::atomic<int64_t> Thread::m_allThreadNum = 0;

Thread::Thread(ThreadFunc threadFunc, const std::string &name)
    : m_running(false)
    , m_joined(false)
    , m_pthreadId(0)
    , m_tid()
    , m_func(std::move(threadFunc))
    , m_name(name)
{
    setDefaultName();
}

Thread::~Thread()
{
    if(m_running && !m_joined){
        pthread_detach(m_pthreadId);
        m_allThreadNum.fetch_sub(1);
    }
}

void Thread::start()
{
    assert(!m_running);
    m_running = true;
    if (pthread_create(&m_pthreadId,
                       nullptr,
                       threadFunc,
                       reinterpret_cast<void*>(this))){
        m_running = false;
        LOG_FATAL << "Failed in pthread_create";
    }
    m_allThreadNum.fetch_add(1);
}

int Thread::join()
{
    assert(m_running);
    assert(!m_joined);
    m_joined = true;
    m_allThreadNum.fetch_sub(1);
    return pthread_join(m_pthreadId, nullptr);
}

int Thread::cancel()
{
    assert(m_running);
    assert(!m_joined);
    m_joined = true;
    m_allThreadNum.fetch_sub(1);
    return pthread_cancel(m_pthreadId);
}

void* Thread::threadFunc(void *obj)
{
    Thread *pThis = static_cast<Thread *>(obj);
    pThis->m_tid = CurrentThread::tid();
    pThis->m_func();
    return 0;
}

void Thread::setDefaultName()
{
    int64_t num = m_numCreated.fetch_add(1);
    if (m_name.empty())
    {
        char buf[32];
        snprintf(buf, sizeof buf, "Thread%ld", num);
        m_name = buf;
    }
}

}

}
