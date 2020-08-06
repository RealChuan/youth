#ifndef LOGFILE_H
#define LOGFILE_H

#include "Thread.h"

#include <youth/core/Object.h>
#include <youth/core/Mutex.h>

#include <memory>

namespace youth
{

using namespace core;
namespace core{
class FileUtil;
}

namespace utils
{

class LogFile : noncopyable
{
public:
    static LogFile* instance();

    static void outputFunc(const char*, int);
    static void flushFunc(void);

    void setRollSize(off_t);
    //void setDelLogFileDays(uint);   //delete log files
    void setBaseFileName(const std::string&);

    //这个方法是用来将日志写入Log文件
    void outputLogFile(const char*, int);
    void flushLogFile();

private:  
    LogFile();
    ~LogFile();

    inline bool rollFile(int);
    //inline void delLogFiles();
    inline std::string getFileName(time_t*);

    int m_count;
    off_t m_rollSize = 1000 * 1000 *1000;    //1G
    //uint m_delLogFileDays;
    std::string m_deleteCmd;
    std::string m_basename;
    time_t m_startTime;
    time_t m_lastRoll;
    Mutex m_mutex;
    std::unique_ptr<FileUtil> m_filePtr;
};

}

}

#endif /* LOGFILE_H */
