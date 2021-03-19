#include "Logging.h"
#include "LogOut.h"
#include "LogFile.h"

#include <youth/core/ProcessMsg.h>

namespace youth
{

namespace utils
{

const static char *arrLevel[] = {"[DEBUG] ", "[INFO] ", "[WARN] ", "[ERROR] ", "[FATAL] "};

Logging::LogLevel static g_LogLevel = Logging::INFO; //默认的日志等级为INFO
int static g_OutputMode = LOGGER_MODE_STDOUT; //默认的输出模式为输出到标准输出

void DefauleOutout(const char *pMsg, int Len)
{
    ::fwrite(pMsg, 1, static_cast<size_t>(Len), stdout);
}

void DefauleFlush(void)
{
    ::fflush(stdout);
}

Logging::outputFunc static g_OutputFunc = DefauleOutout; //默认的日志输出到标准输出
Logging::flushFunc static g_FlushFunc = DefauleFlush;

Logging::Logging(LogLevel level, const char* file, int line, bool outError)
    : m_logOutPtr(new LogOut(arrLevel[level], file, line, outError))
    , m_logLevel(level)
{
}

Logging::~Logging()
{
    //当调用析构的时候，日志流填充下文件名和行号，就是完整的一条日志了
    m_logOutPtr->finishLog();
    g_OutputFunc(m_logOutPtr->getLogStreamBuff(), m_logOutPtr->getLogStreamBuffLen());
    if (m_logLevel == FATAL)
    {
        //如果发生了FATAL错误，那么就终止程序。
        //以便之后重启程序。
        DefauleFlush(); //在此之前先冲刷缓冲区
        abort();
    }
}

LogStream &Logging::stream()
{
    return m_logOutPtr->stream();
}

void Logging::setLogLevel(LogLevel level)
{
    g_LogLevel = level;
}

Logging::LogLevel Logging::getLogLevel()
{
    return g_LogLevel;
}

void Logging::setOutputMode(int iMode)
{
    g_OutputMode = iMode;

    if (g_OutputMode == LOGGER_MODE_STDOUT)
    {
        //事实上，默认的输出就是输出到标准输出，不做处理
        return;
    }
    else if (g_OutputMode == LOGGER_MODE_LOGFILE)
    {
        //仅输出到日志文件
        setOutputFunc(LogFile::outputFunc);
        setFlushFunc(LogFile::flushFunc);
    }
    else if (g_OutputMode == (LOGGER_MODE_STDOUT | LOGGER_MODE_LOGFILE))
    {
        //输出到标准输出和log中去
        setOutputFunc(outputOutAndLog);
        setFlushFunc(flushAll);
    }
    else
    {
        //nothing
    }
}

void Logging::setFileBaseName(const char* _basename)
{
    std::string baseName = ProcessMsg::fileBaseName(_basename);
    baseName += ".";
    LogFile::instance()->setBaseFileName(baseName);
}

void Logging::setOutputFunc(outputFunc Output)
{
    g_OutputFunc = Output;
}

void Logging::setFlushFunc(flushFunc Flush)
{
    g_FlushFunc = Flush;
}

void Logging::outputOutAndLog(const char *pMsg, int Len)
{
    DefauleOutout(pMsg, Len);   //输出到标准输出
    LogFile::instance()->outputLogFile(pMsg, Len);   //输出到日志中
}

void Logging::flushAll()
{
    DefauleFlush();
    LogFile::instance()->flushLogFile();
}

__thread char t_errnobuf[512];

const char *strerror_tl(int savedErrno)
{
    return strerror_r(savedErrno, t_errnobuf, sizeof t_errnobuf);
}

}

}
