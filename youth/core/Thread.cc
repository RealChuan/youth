#include "Thread.hpp"
#include "CurrentThread.h"

#include <assert.h>

namespace youth {

namespace core {

std::atomic<int64_t> Thread::s_numCreated = 0;
std::atomic<int64_t> Thread::s_allThreadNum = 0;

Thread::Thread(ThreadFunc threadFunc, const std::string &name)
    : m_running(false)
    , m_joined(false)
    , m_tid()
    , m_func(std::move(threadFunc))
    , m_name(name)
{
    setDefaultName();
}

Thread::~Thread()
{
    if (m_running && !m_joined) {
        m_thread.detach();
        s_allThreadNum.fetch_sub(1);
    }
}

void Thread::start()
{
    assert(!m_running);
    m_running = true;
    m_thread = std::thread(std::bind(&Thread::threadFunc, this));
    s_allThreadNum.fetch_add(1);
}

void Thread::join()
{
    assert(!m_joined);
    m_joined = true;
    s_allThreadNum.fetch_sub(1);
    if (m_thread.joinable()) {
        m_thread.join();
    }
}

void Thread::threadFunc()
{
    m_running = true;
    m_tid = CurrentThread::tid();
    if (m_func) {
        m_func();
    }
    m_running = false;
}

void Thread::setDefaultName()
{
    int64_t num = s_numCreated.fetch_add(1);
    if (m_name.empty()) {
        char buf[32];
        snprintf(buf, sizeof buf, "Thread%ld", num);
        m_name = buf;
    }
}

} // namespace core

} // namespace youth
