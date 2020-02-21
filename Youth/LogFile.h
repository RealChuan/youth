#ifndef LOGFILE_H
#define LOGFILE_H

#include "Thread.h"
#include "Singleton.h"
//#include "FileUtil.h"

namespace youth
{
class FileUtil;
class LogFile
{
    SINGLETON(LogFile)

    public:
        void setRollSize(off_t);
    void setBaseFileName(const std::string&);
    static void outputFunc(const char*, int);
    static void flushFunc(void);

    //这个方法是用来将日志写入Log文件
    void outputLogFile(const char*, int);
    void flushLogFile();

    bool rollFile();

private:  
    std::string getFileName(time_t*);

    off_t rollSize_ = 1000 * 1000 *1000;    //1G
    time_t startTime;
    time_t lastRoll;
    std::string basename;
    Mutex mutex;
    std::unique_ptr<FileUtil> file;
};
}

#endif /* LOGFILE_H */
