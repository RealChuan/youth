#include "LogFile.h"

#include <youth/core/DateTime.hpp>
#include <youth/core/Dir.h>
#include <youth/core/File.hpp>
#include <youth/core/FileInfo.hpp>
#include <youth/core/Process.hpp>

#include <sys/stat.h>
#include <sys/types.h>

namespace youth {

namespace utils {

const static int kRollPerSeconds_ = 60 * 60 * 24;

LogFile::LogFile()
    : m_count(0)
    , m_startTime(0)
    , m_lastRoll(0)
{
    //rollFile();
    //setDelLogFileDays(7);
}

LogFile::~LogFile() {}

static Mutex g_logFileMutex;

LogFile *LogFile::instance()
{
    MutexLock lock(g_logFileMutex);
    static LogFile logFile;
    return &logFile;
}

void LogFile::setBaseFileName(const std::string &basename_)
{
    if (basename_.empty()) {
        m_basename = Dir::Current().absolutePath().string();
    } else {
        m_basename = basename_;
    }
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
    if (m_filePtr->size() > m_rollSize) {
        rollFile(++m_count);
    } else {
        time_t now = ::time(NULL);
        time_t thisPeriod = now / kRollPerSeconds_ * kRollPerSeconds_;
        if (thisPeriod != m_startTime) {
            m_count = 0;
            rollFile(0);
        }
    }

    m_filePtr->write(msg, len);
}

void LogFile::flushLogFile()
{
    MutexLock lock(m_mutex);
    m_filePtr->flush();
}

std::string LogFile::getFileName(time_t *now)
{
    std::string fileName;

    char buf[32] = {0};
    struct tm tm;
    *now = time(NULL);
    gmtime_r(now, &tm);
    strftime(buf, sizeof buf, "%Y-%m-%d.", &tm);
    fileName = "/Log/" + m_basename + buf + Process::hostname();
    snprintf(buf, sizeof buf, ".%d", Process::getPid());
    fileName = fileName + buf + ".log";

    auto dir = Dir::Current();
    dir.mkdir("Log");
    return dir.absolutePath().string() + fileName;
}

bool LogFile::rollFile(int count)
{
    delLogFile();
    time_t now = 0;
    std::string fileName = getFileName(&now);
    if (count) {
        char buf[32] = {0};
        snprintf(buf, sizeof buf, ".%d", count);
        fileName += buf;
    }
    time_t start = now / kRollPerSeconds_ * kRollPerSeconds_;
    if (now > m_lastRoll) {
        //delLogFiles();
        m_startTime = start;
        m_lastRoll = now;
        m_filePtr.reset(new File(fileName));
        m_filePtr->open(File::Append);
        return true;
    }
    return false;
}

void LogFile::delLogFile()
{
    auto now = DateTime::currentDateTime();
    now.addDays(-m_delLogFileDays);
    auto dir = Dir::Current();
    auto list = dir.entryInfoList({});
    for (auto &file : list) {
        if (file.isFile() && file.lastModified() < now) {
            dir.removeFile(file.fileName());
        }
    }
}

} // namespace utils

} // namespace youth
