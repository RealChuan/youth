#include "Thread.h"
#include "Logging.h"

#include <youth/core/Timestamp.h>

#include <assert.h>

namespace youth
{

namespace utils
{

Thread::Thread(ThreadFunc func_)
    : m_running(false),
      m_joined(false),
      m_pthreadId(0),
      m_func(std::move(func_))
{
}

Thread::~Thread()
{
    if(m_running && !m_joined)
        pthread_detach(m_pthreadId);
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
        perror("Failed in pthread_create");
        exit(-1);
    }
}

int Thread::join()
{
    assert(m_running);
    assert(!m_joined);
    m_joined = true;
    return pthread_join(m_pthreadId, nullptr);
}

bool Thread::isRunning() const
{
    return m_running;
}

pthread_t Thread::pthreadID() const
{
    return m_pthreadId;
}

}

}
