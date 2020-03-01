#ifndef LOGASYNC_H
#define LOGASYNC_H

#include "LogBuffer.h"
#include "vector"
#include "memory"
#include "string"
#include "Thread.h"
#include "../core/Timestamp.h"

namespace youth
{

class LogAsync
{
public:
    LogAsync(int flushInterval);
    ~LogAsync();

    void append(const char* logline, int len);

    void start()
    {
        running = true;
        thread.start();
        printf("日志线程开始\n");
    }

    void stop()
    {
        running = false;
        cond.notify();
        //保证日志全部输出
        {
            MutexLock lock(mutex);
            cond.wait();
        }
        thread.join();
        printf("日志线程结束\n");
    }

private:
    void threadFunc();
    Timestamp timestamp;

    typedef LogBuffer<maxBuffer> Buffer;
    typedef std::vector<std::unique_ptr<Buffer>> BufferVector;
    typedef BufferVector::value_type BufferPtr;

    const int refreshTime;
    bool running;
    Thread thread;
    Mutex mutex;
    Condition cond GUARDED_BY(mutex);
    BufferPtr currentBuffer GUARDED_BY(mutex);
    BufferPtr nextBuffer GUARDED_BY(mutex);
    BufferVector buffers GUARDED_BY(mutex);
};

}
#endif /* LOGASYNC_H */
