#include "ThreadPool.h"

#include <assert.h>

namespace youth {

namespace utils {

ThreadPool::ThreadPool()
    : m_maxTaskNum(0), m_running(false), m_mutex(), m_notEmpty(), m_notFull() {}

ThreadPool::~ThreadPool() {
  if (m_running) {
    stop();
  }
}

void ThreadPool::start(int numThreads) {
  assert(m_threadVec.empty());
  m_running = true;
  m_threadVec.reserve(static_cast<unsigned long>(numThreads));
  for (int i = 0; i < numThreads; ++i) {
    char id[32];
    snprintf(id, sizeof id, "%d", i + 1);
    m_threadVec.emplace_back(
        new Thread(std::bind(&ThreadPool::threadFunc, this)));
    m_threadVec[static_cast<unsigned long>(i)]->start();
  }
  if (numThreads == 0 && m_threadInitCallback) {
    m_threadInitCallback();
  }
}

void ThreadPool::stop() {
  {
    MutexLock lock(m_mutex);
    m_running = false;
    m_notEmpty.notify_all();
  }
  for (auto &thr : m_threadVec) {
    thr->join();
  }
}

void ThreadPool::threadFunc() {
  assert(m_running == true);

  if (m_threadInitCallback) {
    m_threadInitCallback();
  }
  while (m_running) {
    Task task(take());
    if (task) {
      task();
    }
  }
}

void ThreadPool::run(Task task) {
  if (m_threadVec.empty()) {
    task();
  } else {
    MutexLock lock(m_mutex);
    while (isFull()) {
      m_notFull.wait(lock);
    }
    assert(!isFull());

    m_queue.push_back(std::move(task));
    m_notEmpty.notify_one();
  }
}

ThreadPool::Task ThreadPool::take() {
  MutexLock lock(m_mutex);
  // always use a while-loop, due to spurious wakeup
  while (m_queue.empty() && m_running) {
    m_notEmpty.wait(lock);
  }
  Task task;
  if (!m_queue.empty()) {
    task = m_queue.front();
    m_queue.pop_front();
    if (m_maxTaskNum > 0) {
      m_notFull.notify_one();
    }
  }
  return task;
}

} // namespace utils

} // namespace youth
