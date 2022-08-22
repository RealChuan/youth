#include "LogAsync.h"
#include "LogFile.h"
#include "Logging.h"

#include <assert.h>

namespace youth {

namespace utils {

void asyncOutput(const char *msg, int len)
{
    LogAsync::instance().append(msg, len);
}

LogAsync::LogAsync()
    : m_dateTime(DateTime::currentDateTime())
    , m_refreshTime(5)
    , m_running(false)
    , m_thread(std::bind(&LogAsync::threadFunc, this))
    , m_mutex()
    , m_cond()
    , m_currentBuffer(new Buffer)
    , m_nextBuffer(new Buffer)
    , m_buffers()
{
    m_currentBuffer->bzero();
    m_nextBuffer->bzero();
    m_buffers.reserve(16);
    Logging::setOutputFunc(asyncOutput);
}

LogAsync::~LogAsync()
{
    if (m_running) {
        stop();
    }
}

void LogAsync::append(const char *buf, int len)
{
    MutexLock lock(m_mutex);
    if (m_currentBuffer->avail() > len) {
        m_currentBuffer->append(buf, len);
    } else {
        m_buffers.push_back(std::move(m_currentBuffer));

        if (m_nextBuffer) {
            m_currentBuffer = std::move(m_nextBuffer);
        } else {
            m_currentBuffer.reset(new Buffer); // Rarely happens
        }
        m_currentBuffer->append(buf, len);
        m_cond.notify_one();
    }
}

void LogAsync::start()
{
    m_running = true;
    m_thread.start();
    printf("日志线程开始\n");
}

void LogAsync::stop()
{
    m_running = false;
    m_cond.notify_one();
    //保证日志全部输出
    {
        MutexLock lock(m_mutex);
        m_cond.wait(lock);
    }
    m_thread.join();
    printf("日志线程结束\n");
}

void LogAsync::threadFunc()
{
    assert(m_running == true);
    BufferPtr newBuffer1(new Buffer);
    BufferPtr newBuffer2(new Buffer);
    newBuffer1->bzero();
    newBuffer2->bzero();
    BufferVector buffersToWrite;
    buffersToWrite.reserve(16);
    while (m_running) {
        assert(newBuffer1 && newBuffer1->length() == 0);
        assert(newBuffer2 && newBuffer2->length() == 0);
        assert(buffersToWrite.empty());

        {
            MutexLock lock(m_mutex);
            if (m_buffers.empty()) // unusual usage!
            {
                m_cond.wait_for(lock, std::chrono::milliseconds(m_refreshTime));
            }

            m_buffers.push_back(std::move(m_currentBuffer));
            m_currentBuffer = std::move(newBuffer1);
            buffersToWrite.swap(m_buffers);
            if (!m_nextBuffer) {
                m_nextBuffer = std::move(newBuffer2);
            }
        }
        assert(!buffersToWrite.empty());

        if (buffersToWrite.size() > 25) {
            char buf[256];
            snprintf(buf,
                     sizeof buf,
                     "Dropped log messages at %s, %zd larger buffers\n",
                     m_dateTime.toStandardFormat().c_str(),
                     buffersToWrite.size() - 2);
            fputs(buf, stderr);
            LogFile::outputFunc(buf, static_cast<int>(strlen(buf)));
            buffersToWrite.erase(buffersToWrite.begin() + 2, buffersToWrite.end());
        }

        for (const auto &buffer : buffersToWrite) {
            // FIXME: use unbuffered stdio FILE ? or use ::writev ?
            LogFile::outputFunc(buffer->data(), buffer->length());
        }

        if (buffersToWrite.size() > 2) {
            // drop non-bzero-ed buffers, avoid trashing
            buffersToWrite.resize(2);
        }

        if (!newBuffer1) {
            assert(!buffersToWrite.empty());
            newBuffer1 = std::move(buffersToWrite.back());
            buffersToWrite.pop_back();
            newBuffer1->reset();
        }

        if (!newBuffer2) {
            assert(!buffersToWrite.empty());
            newBuffer2 = std::move(buffersToWrite.back());
            buffersToWrite.pop_back();
            newBuffer2->reset();
        }

        buffersToWrite.clear();
        LogFile::flushFunc();
    }
    LogFile::flushFunc();
    m_cond.notify_one();
}

} // namespace utils

} // namespace youth
