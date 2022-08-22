#ifndef LOGFILE_H
#define LOGFILE_H

#include "Singleton.h"

#include <youth/core/Thread.hpp>

#include <memory>

namespace youth {

namespace core {
class File;
class DateTime;
} // namespace core

namespace utils {

class LogFile : noncopyable
{
    SINGLETON(LogFile)
public:
    void setRollSize(off_t size) { m_rollSize = size; }
    void setBaseFileName(const std::string &);
    void setMaxFileSize(off_t size) { m_rollSize = size; }
    void setDelLogFileDays(uint days) { m_delLogFileDays = days; }

    //这个方法是用来将日志写入Log文件
    void outputLogFile(const char *, int);
    void flushLogFile();

    static void outputFunc(const char *msg, int len)
    {
        LogFile::instance().outputLogFile(msg, len);
    }
    static void flushFunc() { LogFile::instance().flushLogFile(); }

private:
    bool rollFile(int);
    std::string getFileName(const DateTime &);
    void delLogFile(const DateTime &dateTime);

    int m_count;
    off_t m_rollSize = 1000 * 1000 * 1000; //1G
    int64_t m_delLogFileDays = 7;
    std::string m_basename;
    int64_t m_startTime; //secondsSinceEpoch
    int64_t m_lastRoll;  //secondsSinceEpoch
    Mutex m_mutex;
    std::unique_ptr<core::File> m_filePtr;
};

} // namespace utils

} // namespace youth

#endif /* LOGFILE_H */
