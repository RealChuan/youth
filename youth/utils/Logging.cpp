#include "Logging.h"
#include "LogFile.h"
#include "LogOut.h"

#include <youth/core/Process.hpp>

namespace youth {

namespace utils {

Logging::LogLevel Logging::s_logLevel = Logging::INFO; //默认的日志等级为INFO
int Logging::s_outputModel = LOGGER_MODE_STDOUT; //默认的输出模式为输出到标准输出

const char *Logging::s_arrLevel[] = {"[DEBUG] ", "[INFO] ", "[WARN] ", "[ERROR] ", "[FATAL] "};

Logging::outputFunc Logging::s_outputFunc = Logging::defaultOutputFunc; //默认的日志输出到标准输出
Logging::flushFunc Logging::s_flushFunc = Logging::defaultFlushFunc;

Logging::Logging(LogLevel level, const char *file, int line, bool outError)
    : m_logOutPtr(new LogOut(s_arrLevel[level], file, line, outError))
    , m_logLevel(level)
{}

Logging::~Logging()
{
    //当调用析构的时候，日志流填充下文件名和行号，就是完整的一条日志了
    m_logOutPtr->finishLog();
    s_outputFunc(m_logOutPtr->getLogStreamBuff(), m_logOutPtr->getLogStreamBuffLen());
    if (m_logLevel != FATAL) {
        return;
    }
    //如果发生了FATAL错误，那么就终止程序。
    //以便之后重启程序。
    if (s_flushFunc) {
        s_flushFunc(); //在此之前先冲刷缓冲区
    }
    abort();
}

LogStream &Logging::stream()
{
    return m_logOutPtr->stream();
}

void Logging::setOutputMode(int iMode)
{
    s_outputModel = iMode;

    if (s_outputModel == LOGGER_MODE_STDOUT) {
        return; //事实上，默认的输出就是输出到标准输出，不做处理
    } else if (s_outputModel == LOGGER_MODE_LOGFILE) {
        //仅输出到日志文件
        setOutputFunc(LogFile::outputFunc);
        setFlushFunc(LogFile::flushFunc);
    } else if (s_outputModel == (LOGGER_MODE_STDOUT | LOGGER_MODE_LOGFILE)) {
        //输出到标准输出和log中去
        setOutputFunc(outputOutAndLog);
        setFlushFunc(flushAll);
    } else {
        //nothing
    }
}

void Logging::setFileBaseName(const char *_basename)
{
    std::string baseName = Process::fileBaseName(_basename);
    baseName += ".";
    LogFile::instance().setBaseFileName(baseName);
}

void Logging::defaultOutputFunc(const char *pMsg, int Len)
{
    ::fwrite(pMsg, 1, static_cast<size_t>(Len), stdout);
}

void Logging::defaultFlushFunc()
{
    ::fflush(stdout);
}

void Logging::outputOutAndLog(const char *pMsg, int Len)
{
    defaultOutputFunc(pMsg, Len);                 //输出到标准输出
    LogFile::instance().outputLogFile(pMsg, Len); //输出到日志中
}

void Logging::flushAll()
{
    defaultFlushFunc();
    LogFile::instance().flushLogFile();
}

__thread char t_errnobuf[512];

const char *strerror_tl(int savedErrno)
{
    return strerror_r(savedErrno, t_errnobuf, sizeof t_errnobuf);
}

} // namespace utils

} // namespace youth
