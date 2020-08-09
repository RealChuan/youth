#ifndef LOGASYNC_H
#define LOGASYNC_H

#include "LogBuffer.h"
#include "Thread.h"

#include <youth/core/Timestamp.h>
#include <youth/core/Condition.h>

#include <vector>
#include <memory>

namespace youth
{

namespace utils
{

class LogAsync : noncopyable
{
public:
    static LogAsync* instance();

    void setFlushInterval(int flushInterval);

    void append(const char* logline, int len);

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
    Condition m_cond GUARDED_BY(m_mutex);
    BufferPtr m_currentBuffer GUARDED_BY(m_mutex);
    BufferPtr m_nextBuffer GUARDED_BY(m_mutex);
    BufferVector m_buffers GUARDED_BY(m_mutex);
};

}

}
#endif /* LOGASYNC_H */
