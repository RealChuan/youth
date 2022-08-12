#include "CountDownLatch.h"

namespace youth {

namespace core {

CountDownLatch::CountDownLatch(int count)
    : m_mutex(), m_condition(), m_count(count) {}

void CountDownLatch::wait() {
  MutexLock lock(m_mutex);
  while (m_count > 0) {
    m_condition.wait(lock);
  }
}

void CountDownLatch::countDown() {
  MutexLock lock(m_mutex);
  m_count--;
  if (m_count == 0) {
    m_condition.notify_all();
  }
}

int CountDownLatch::getCount() const {
  MutexLock lock(m_mutex);
  return m_count;
}

} // namespace core

} // namespace youth
