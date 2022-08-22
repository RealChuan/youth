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
    Dir::mkdirs("./Log");
}

LogFile::~LogFile() {}

void LogFile::setBaseFileName(const std::string &basename)
{
    if (basename.empty()) {
        m_basename = Dir::Current().dirname();
    } else {
        m_basename = basename;
    }
    rollFile(0);
}

void LogFile::outputLogFile(const char *msg, int len)
{
    MutexLock lock(m_mutex);

    //roll file
    if (m_filePtr->size() > m_rollSize) {
        rollFile(++m_count);
    } else {
        auto now = DateTime::currentSecondsSinceEpoch();
        auto thisPeriod = now / kRollPerSeconds_ * kRollPerSeconds_;
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

std::string LogFile::getFileName(const DateTime &dateTime)
{
    return "./Log/" + m_basename + dateTime.toString("%Y%m%d-%H%M%S") + Process::hostname() + "."
           + std::to_string(Process::getPid()) + ".log";
}

bool LogFile::rollFile(int count)
{
    auto now = DateTime::currentDateTime();
    std::string fileName = getFileName(now);
    if (count) {
        char buf[32] = {0};
        snprintf(buf, sizeof buf, ".%d", count);
        fileName += buf;
    } else {
        delLogFile(now);
    }
    int64_t seconds = now.secondsSinceEpoch();
    auto start = seconds / kRollPerSeconds_ * kRollPerSeconds_;
    if (seconds > m_lastRoll) {
        m_startTime = start;
        m_lastRoll = seconds;
        m_filePtr.reset(new File(fileName));
        m_filePtr->open(File::Append);
        return true;
    }
    return false;
}

void LogFile::delLogFile(const DateTime &dateTime)
{
    auto expired = dateTime.addDays(-m_delLogFileDays);
    Dir dir("./Log");
    auto list = dir.entryInfoList({});
    for (auto &file : list) {
        if (file.isFile() && file.lastModified() < expired) {
            dir.removeFile(file.fileName());
        }
    }
}

} // namespace utils

} // namespace youth
