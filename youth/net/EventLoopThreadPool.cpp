#include "EventLoopThreadPool.h"

#include <assert.h>

#include "EventLoop.h"
#include "EventLoopThread.h"

namespace youth {

namespace net {

EventLoopThreadPool::EventLoopThreadPool(EventLoop *baseLoop,
                                         const std::string &name)
    : m_baseLoop(baseLoop),
      m_name(name),
      m_started(false),
      m_numThreads(0),
      m_next(0) {}

EventLoopThreadPool::~EventLoopThreadPool() {}

void EventLoopThreadPool::start(
    const EventLoopThreadPool::ThreadInitCallback &cb) {
  assert(!m_started);
  m_baseLoop->assertInLoopThread();

  m_started = true;

  for (int i = 0; i < m_numThreads; ++i) {
    char buf[m_name.size() + 32];
    snprintf(buf, sizeof buf, "%s%d", m_name.c_str(), i);
    EventLoopThread *t = new EventLoopThread(cb, buf);
    m_threadPtrs.push_back(std::unique_ptr<EventLoopThread>(t));
    m_loops.push_back(t->startLoop());
  }
  if (m_numThreads == 0 && cb) {
    cb(m_baseLoop);
  }
}

EventLoop *EventLoopThreadPool::getNextLoop() {
  m_baseLoop->assertInLoopThread();
  assert(m_started);
  EventLoop *loop = m_baseLoop;

  if (!m_loops.empty()) {
    // round-robin
    loop = m_loops[m_next];
    ++m_next;
    if (size_t(m_next) >= m_loops.size()) {
      m_next = 0;
    }
  }
  return loop;
}

EventLoop *EventLoopThreadPool::getLoopForHash(size_t hashCode) {
  m_baseLoop->assertInLoopThread();
  EventLoop *loop = m_baseLoop;

  if (!m_loops.empty()) {
    loop = m_loops[hashCode % m_loops.size()];
  }
  return loop;
}

std::vector<EventLoop *> EventLoopThreadPool::getAllLoops() {
  m_baseLoop->assertInLoopThread();
  assert(m_started);
  if (m_loops.empty()) {
    return std::vector<EventLoop *>(1, m_baseLoop);
  } else {
    return m_loops;
  }
}

}  // namespace net

}  // namespace youth
