#include "LogFile.h"
#include "ProcessMsg.h"
#include "FileUtil.h"

#include <sys/stat.h>
#include <sys/types.h>

using namespace youth;

const static int kRollPerSeconds_ = 60*60*24;

LogFile::LogFile()
    :startTime(0)
    ,lastRoll(0)
{
    //rollFile();
}

LogFile::~LogFile()
{
}

void LogFile::setRollSize(off_t size)
{
    rollSize_ = size;
}

void LogFile::setBaseFileName(const std::string &basename_)
{
    basename = basename_;
    rollFile();
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
    if (file->writeBytes() > rollSize_){
        rollFile();
    }
    else{
        time_t now = ::time(NULL);
        time_t thisPeriod_ = now / kRollPerSeconds_ * kRollPerSeconds_;
        if (thisPeriod_ != startTime){
            rollFile();
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

bool LogFile::rollFile()
{
    time_t now = 0;
    std::string fileName = getFileName(&now);
    time_t start = now / kRollPerSeconds_ * kRollPerSeconds_;
    if (now > lastRoll)
    {
        startTime = start;
        lastRoll = now;
        file.reset(new FileUtil(fileName));
        file->open(FileUtil::Write);
        return true;
    }
    return false;
}
