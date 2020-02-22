#include "LogFile.h"
#include "ProcessMsg.h"
#include "FileUtil.h"

#include <sys/stat.h>
#include <sys/types.h>

using namespace youth;

const static int kRollPerSeconds_ = 60*60*24;

LogFile::LogFile()
    :count(0)
    ,startTime(0)
    ,lastRoll(0)
{
    //rollFile();
    setDelLogFileDays(7);
}

LogFile::~LogFile()
{
}

void LogFile::setRollSize(off_t size)
{
    rollSize = size;
}

void LogFile::setDelLogFileDays(uint days)
{
    delLogFileDays = days;
    char pathBuf[64] = {0};
    snprintf(pathBuf, sizeof pathBuf, "%s/%s", get_current_dir_name(), "Log");
    char cmd[128] = {0};
    sprintf(cmd, "find %s/* -mtime +%d -exec rm -rf {} \\;", pathBuf, delLogFileDays);    //删除上一次修改时间超过3天的文件
    //printf("Record: %s.\n", cmd);
    deleteCmd = cmd;
}

void LogFile::setBaseFileName(const std::string &basename_)
{
    basename = basename_;
    rollFile(0);
}

void LogFile::outputFunc(const char *msg, int len)
{
    LogFile *pLogFile = &instance();
    pLogFile->outputLogFile(msg, len);
}

void LogFile::flushFunc()
{
    LogFile *pLogFile = &instance();
    pLogFile->flushLogFile();
}

void LogFile::outputLogFile(const char *msg, int len)
{
    MutexLock lock(mutex);
    file->write(msg, len);

    //roll file
    if (file->writeBytes() > rollSize){
        rollFile(++count);
    }
    else{
        time_t now = ::time(NULL);
        time_t thisPeriod = now / kRollPerSeconds_ * kRollPerSeconds_;
        if (thisPeriod != startTime){
            count = 0;
            rollFile(0);
        }
    }
}

void LogFile::flushLogFile()
{
    MutexLock lock(mutex);
    file->flushFile();
}

std::string LogFile::getFileName(time_t *now)
{  
    std::string fileName;

    char buf[32] = {0};
    struct tm tm;
    *now = time(NULL);
    gmtime_r(now, &tm);
    strftime(buf, sizeof buf, "%Y-%m-%d.", &tm);
    fileName = "Log/" + basename + buf + processmsg::hostname();
    snprintf(buf, sizeof buf, ".%d", processmsg::getPid());
    fileName = fileName + buf + ".log";

    return fileName;
}

bool LogFile::rollFile(int count)
{
    time_t now = 0;
    std::string fileName = getFileName(&now);
    if(count){
        char buf[5] = {0};
        snprintf(buf, sizeof buf, ".%d", count);
        fileName += buf;
    }
    time_t start = now / kRollPerSeconds_ * kRollPerSeconds_;
    if (now > lastRoll)
    {
        delLogFiles();
        startTime = start;
        lastRoll = now;
        file.reset(new FileUtil(fileName));
        file->open(FileUtil::Append);
        return true;
    }
    return false;
}

void LogFile::delLogFiles()
{
    system(deleteCmd.c_str());
    //perror("Delete LogFile Failed!");
}
