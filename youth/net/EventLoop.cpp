#include "EventLoop.h"
#include "youth/utils/LogOut.h"
#include "youth/core/CurrentThread.h"

#include <assert.h>

using namespace youth;

__thread EventLoop* g_loopInThisThread = nullptr;

EventLoop::EventLoop()
    : m_threadID(CurrentThread::tid())
    , m_looping(false)
    , m_quit(false)
{
    LOG_DEBUG << "EventLoop created " << this << " in thread " << m_threadID;
    if(g_loopInThisThread)
    {
        LOG_FATAL << "Another EventLoop " << g_loopInThisThread
                  << " exists in this thread " << m_threadID;
    }
    else
    {
        g_loopInThisThread = this;
    }
}

EventLoop::~EventLoop()
{
    g_loopInThisThread = nullptr;
}

void EventLoop::loop()
{
    assert(!m_looping);
    assertInLoopThread();
    m_looping = true;
    m_quit = false;  // FIXME: what if someone calls quit() before loop() ?
    LOG_DEBUG << "EventLoop " << this << " start looping";
    while (!m_quit)
    {
        // Todo loop
    }

}

void EventLoop::quit()
{
    m_quit = true;
}

bool EventLoop::isInLoopThread() const
{
    return m_threadID == CurrentThread::tid();
}

void EventLoop::abortNotInLoopThread()
{
    LOG_FATAL << "EventLoop::abortNotInLoopThread - EventLoop " << this
              << " was created in thread id = " << m_threadID
              << ", current thread id = " <<  CurrentThread::tid();
}

void EventLoop::assertInLoopThread()
{
    if (!isInLoopThread())
    {
        abortNotInLoopThread();
    }
}
