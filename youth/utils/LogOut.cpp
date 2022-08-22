#include "LogOut.h"
#include "LogAsync.h"
#include "LogFile.h"

#include <youth/core/CurrentThread.h>

#include <error.h>

namespace youth {

namespace utils {

LogOut::LogOut(const char *level, const char *file, int line, bool outError)
    : m_line(line)
    , m_file(file)
    , m_dateTime(DateTime::currentDateTime())
    , m_logStream()
{
    CurrentThread::tid();
    //在构造的时候会填充日志头(其实就是时间和报警等级)
    //首先，输出打印日志时间到LogStream中的Buffer
    //其次，输出日志等级
    m_logStream << m_dateTime.toStandardFormat() << " " << CurrentThread::tidString() << " "
                << level;
    if (outError) {
        m_logStream << " Error: " << errno << " " << strerror(errno) << ". ";
    }
}

LogOut::~LogOut() {}

void LogOut::finishLog()
{
    //根据我们的日志格式安排，最后输出的是
    //文件名即行号。这个方法是在Logger析构
    //时候调用的，表示一条日志流的完成，可以
    //输出到标准输出或文件中去。
    m_logStream << " - " << m_file << ":" << m_line << "\n";
}

const char *LogOut::getLogStreamBuff()
{
    return m_logStream.GetStreamBuff();
}

int LogOut::getLogStreamBuffLen()
{
    return m_logStream.GetStreamBuffLen();
}

} // namespace utils

} // namespace youth
