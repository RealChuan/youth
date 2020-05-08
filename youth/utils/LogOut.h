#ifndef LOGOUT_H
#define LOGOUT_H

#include "LogStream.h"
#include "youth/core/Timestamp.h"
#include "youth/core/Object.h"

#include <memory>

namespace youth
{

class LogOut : public noncopyable
{
public:
    LogOut(const char* pLevel, const char* File, int Line);
    virtual ~LogOut();
    LogStream &getLogStream();

    //这个方法是在完成日志流的输出后填充文件名和行号的
    void finishLog();

    //我们的实现上，日志流是先存储在固定的
    //Buffer里面，等到整条日志流输出完毕。
    //这个方法就是获取获取日志流
    const char *getLogStreamBuff();
    int getLogStreamBuffLen();

private:
    int line; //行号
    const char* file;
    Timestamp m_timestamp; //Log时间类
    LogStream logStream; //LogStream类
};

class LogFile;
class Logging
{
public:
    //DEBUG Level:指出细粒度信息事件对调试应用程序是非常有帮助的。
    //INFO Level:消息在粗粒度级别上突出强调应用程序的运行
    //WARN Level:表明会出现潜在错误的情况。
    //ERROR Level:指出虽然发生错误事件，但仍然不影响系统的继续运行。
    //FATAL Level:指出每个严重的错误事件将会导致应用程序的退出。
    //当我们定义了日志的优先级之后，应用程序中比设置的优先级别低的
    //都会被输出。

    enum LogLevel
    {
        DEBUG, INFO, WARN, ERROR, FATAL
    };

    typedef void(*outputFunc)(const char *pMsg, int Len);
    typedef void(*flushFunc)(void);

    //设置和获取日志等级
    static void setLogLevel(LogLevel level);
    static LogLevel getLogLevel();
    //设置输出模式的，支持3种输出模式
    static void setOutputMode(int iMode);
    static void setFileBaseName(const char*);

    Logging(LogLevel Level, const char* _File, int Line);
    ~Logging();

    LogStream &getLogStream();

    static void setOutputFunc(outputFunc Func);

private:
    static void setFlushFunc(flushFunc Func);

    static void outputOutAndLog(const char *pMsg, int Len);
    static void flushAll();

    std::unique_ptr<LogOut> m_LogOut;
    LogLevel logLevel;
};

//这几个宏是用来设置输出模式的：输出到标准输出 输出到log文件 输出到标准输出和log文件
#define LOGGER_MODE_STDOUT  0x01
#define LOGGER_MODE_LOGFILE  0x02
#define LOGGER_MODE_OUTANDFILE (LOGGER_MODE_STDOUT | LOGGER_MODE_LOGFILE)

#define LOG_DEBUG if (Logging::getLogLevel() <= Logging::DEBUG) \
    Logging(Logging::DEBUG, __FILE__,__LINE__).getLogStream()

#define LOG_INFO if (Logging::getLogLevel() <= Logging::INFO) \
    Logging(Logging::INFO, __FILE__,__LINE__).getLogStream()

#define LOG_WARN if (Logging::getLogLevel() <= Logging::WARN) \
    Logging(Logging::WARN,__FILE__, __LINE__).getLogStream()

#define LOG_ERROR  Logging(Logging::ERROR, __FILE__,__LINE__).getLogStream()

#define LOG_FATAL  Logging(Logging::FATAL,__FILE__, __LINE__).getLogStream()

}

#endif /* LOGOUT_H */
