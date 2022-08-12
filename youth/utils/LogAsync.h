#ifndef LOGASYNC_H
#define LOGASYNC_H

#include "LogBuffer.h"

#include <youth/core/Thread.hpp>
#include <youth/core/Timestamp.h>

#include <memory>
#include <vector>

namespace youth {

namespace utils {

class LogAsync : noncopyable {
public:
  static LogAsync *instance();

  void setFlushInterval(int flushInterval) { m_refreshTime = flushInterval; }

  void append(const char *logline, int len);

  void start();
  void stop();

private:
  LogAsync(int flushInterval = 5);
  ~LogAsync();

  void threadFunc();
  Timestamp m_timestamp;

  typedef LogBuffer<maxBuffer> Buffer;
  typedef std::vector<std::unique_ptr<Buffer>> BufferVector;
  typedef BufferVector::value_type BufferPtr;

  int m_refreshTime;
  bool m_running = false;
  Thread m_thread;
  Mutex m_mutex;
  Condition m_cond;
  BufferPtr m_currentBuffer;
  BufferPtr m_nextBuffer;
  BufferVector m_buffers;
};

} // namespace utils

} // namespace youth
#endif /* LOGASYNC_H */
