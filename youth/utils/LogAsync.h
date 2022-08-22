#ifndef LOGASYNC_H
#define LOGASYNC_H

#include "LogBuffer.h"
#include "Singleton.h"

#include <youth/core/DateTime.hpp>
#include <youth/core/Thread.hpp>

#include <memory>
#include <vector>

namespace youth {

namespace utils {

class LogAsync : noncopyable
{
    SINGLETON(LogAsync)
public:
    void setFlushInterval(int flushInterval) { m_refreshTime = flushInterval; }

    void append(const char *logline, int len);

    void start();
    void stop();

private:
    void threadFunc();

    DateTime m_dateTime;

    typedef LogBuffer<maxBuffer> Buffer;
    typedef std::vector<std::unique_ptr<Buffer>> BufferVector;
    typedef BufferVector::value_type BufferPtr;

    int m_refreshTime = 5;
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
