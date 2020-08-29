#include "LogFile.h"

#include <youth/core/ProcessMsg.h>
#include <youth/core/FileUtil.h>
#include <youth/core/Dir.h>

#include <sys/stat.h>
#include <sys/types.h>

namespace youth
{

namespace utils
{

const static int kRollPerSeconds_ = 60*60*24;

LogFile::LogFile()
    :m_count(0)
    ,m_startTime(0)
    ,m_lastRoll(0)
{
    //rollFile();
    //setDelLogFileDays(7);
}

LogFile::~LogFile()
{
}

static Mutex g_logFileMutex;

LogFile *LogFile::instance()
{
    MutexLock lock(g_logFileMutex);
    static LogFile logFile;
    return &logFile;
}

//void LogFile::setDelLogFileDays(uint days)
//{
//    delLogFileDays = days;
//    char pathBuf[64] = {0};
//    snprintf(pathBuf, sizeof pathBuf, "%s/%s", Dir::currentPath().c_str(), "Log");
//    char cmd[128] = {0};
//    sprintf(cmd, "find %s/* -mtime +%d -exec rm -rf {} \\;", pathBuf, delLogFileDays);    //删除上一次修改时间超过3天的文件
//    //printf("Record: %s.\n", cmd);
//    deleteCmd = cmd;
//}

void LogFile::setBaseFileName(const std::string &basename_)
{
    if(basename_.empty())
        m_basename = Dir::currentPath().c_str();
    else
        m_basename = basename_;
    rollFile(0);
}

void LogFile::outputFunc(const char *msg, int len)
{
    LogFile *logFile = LogFile::instance();
    logFile->outputLogFile(msg, len);
}

void LogFile::flushFunc()
{
    LogFile *logFile = LogFile::instance();
    logFile->flushLogFile();
}

void LogFile::outputLogFile(const char *msg, int len)
{
    MutexLock lock(m_mutex);

    //roll file
    if (m_filePtr->writeBytes() > m_rollSize)
    {
        rollFile(++m_count);
    }
    else
    {
        time_t now = ::time(NULL);
        time_t thisPeriod = now / kRollPerSeconds_ * kRollPerSeconds_;
        if (thisPeriod != m_startTime)
        {
            m_count = 0;
            rollFile(0);
        }
    }

    m_filePtr->write(msg, len);
}

void LogFile::flushLogFile()
{
    MutexLock lock(m_mutex);
    m_filePtr->flushFile();
}

std::string LogFile::getFileName(time_t *now)
{  
    std::string fileName;

    char buf[32] = {0};
    struct tm tm;
    *now = time(NULL);
    gmtime_r(now, &tm);
    strftime(buf, sizeof buf, "%Y-%m-%d.", &tm);
    fileName = "Log/" + m_basename + buf + ProcessMsg::hostname();
    snprintf(buf, sizeof buf, ".%d", ProcessMsg::getPid());
    fileName = fileName + buf + ".log";

    return fileName;
}

bool LogFile::rollFile(int count)
{
    time_t now = 0;
    std::string fileName = getFileName(&now);
    if(count)
    {
        char buf[5] = {0};
        snprintf(buf, sizeof buf, ".%d", count);
        fileName += buf;
    }
    time_t start = now / kRollPerSeconds_ * kRollPerSeconds_;
    if (now > m_lastRoll)
    {
        //delLogFiles();
        m_startTime = start;
        m_lastRoll = now;
        m_filePtr.reset(new FileUtil(fileName));
        m_filePtr->open(FileUtil::Append);
        return true;
    }
    return false;
}

//void LogFile::delLogFiles()
//{
//    system(deleteCmd.c_str());
//    //perror("Delete LogFile Failed!");
//}

}

}
