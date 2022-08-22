#ifndef LOGGING_H
#define LOGGING_H

#include "LogStream.h"

#include <memory>

namespace youth {

namespace utils {

class LogFile;
class LogStream;
class LogOut;
class Logging : noncopyable
{
public:
    //DEBUG Level: 指出细粒度信息事件对调试应用程序是非常有帮助的。
    //INFO Level: 消息在粗粒度级别上突出强调应用程序的运行
    //WARN Level: 表明会出现潜在错误的情况。
    //ERROR Level: 指出虽然发生错误事件，但仍然不影响系统的继续运行。
    //FATAL Level: 指出每个严重的错误事件将会导致应用程序的退出。
    //当我们定义了日志的优先级之后，应用程序中比设置的优先级别低的
    //都会被输出。
    enum LogLevel { DEBUG, INFO, WARN, ERROR, FATAL };
    using outputFunc = std::function<void(const char *pMsg, int Len)>;
    using flushFunc = std::function<void()>;

    Logging(LogLevel level, const char *file, int line, bool outError = false);
    ~Logging();

    LogStream &stream();

    //设置和获取日志等级
    static void setLogLevel(LogLevel level) { s_logLevel = level; }
    static LogLevel getLogLevel() { return s_logLevel; }
    //设置输出模式的，支持3种输出模式
    static void setOutputMode(int iMode);
    static void setFileBaseName(const char *);

    static void setOutputFunc(outputFunc Func) { s_outputFunc = Func; }

private:
    static void setFlushFunc(flushFunc Func) { s_flushFunc = Func; }

    static void defaultOutputFunc(const char *pMsg, int Len);
    static void defaultFlushFunc();

    static void outputOutAndLog(const char *pMsg, int Len);
    static void flushAll();

    static LogLevel s_logLevel;
    static int s_outputModel;

    const static char *s_arrLevel[];

    static Logging::outputFunc s_outputFunc;
    static Logging::flushFunc s_flushFunc;

    std::unique_ptr<LogOut> m_logOutPtr;
    LogLevel m_logLevel;
};

const char *strerror_tl(int savedErrno);

} // namespace utils

} // namespace youth

//这几个宏是用来设置输出模式的：输出到标准输出 输出到log文件 输出到标准输出和log文件
#define LOGGER_MODE_STDOUT 0x01
#define LOGGER_MODE_LOGFILE 0x02
#define LOGGER_MODE_OUTANDFILE (LOGGER_MODE_STDOUT | LOGGER_MODE_LOGFILE)

#define LOG_DEBUG \
    if (Logging::getLogLevel() <= Logging::DEBUG) \
    Logging(Logging::DEBUG, __FILE__, __LINE__).stream()

#define LOG_INFO \
    if (Logging::getLogLevel() <= Logging::INFO) \
    Logging(Logging::INFO, __FILE__, __LINE__).stream()

#define LOG_WARN \
    if (Logging::getLogLevel() <= Logging::WARN) \
    Logging(Logging::WARN, __FILE__, __LINE__).stream()

#define LOG_ERROR Logging(Logging::ERROR, __FILE__, __LINE__, true).stream()

#define LOG_FATAL Logging(Logging::FATAL, __FILE__, __LINE__, true).stream()

// Taken from glog/logging.h
//
// Check that the input is non NULL.  This very useful in constructor
// initializer lists.

#define CHECK_NOTNULL(val) CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))

// A small helper for CHECK_NOTNULL().
template<typename T>
T *CheckNotNull(const char *file, int line, const char *names, T *ptr)
{
    if (ptr == NULL) {
        youth::utils::Logging(youth::utils::Logging::FATAL, file, line, true).stream() << names;
    }
    return ptr;
}

#endif // LOGGING_H
