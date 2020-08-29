#ifndef LOGOUT_H
#define LOGOUT_H

#include "LogStream.h"

#include <youth/core/Timestamp.h>

namespace youth
{

namespace utils
{

class LogOut : noncopyable
{
public:
    LogOut(const char* level, const char* m_file, int m_line, bool outError = false);
    ~LogOut();

    LogStream& stream() { return m_logStream; }

    //这个方法是在完成日志流的输出后填充文件名和行号的
    void finishLog();

    //我们的实现上，日志流是先存储在固定的
    //Buffer里面，等到整条日志流输出完毕。
    //这个方法就是获取获取日志流
    const char *getLogStreamBuff();
    int getLogStreamBuffLen();

private:
    int m_line; //行号
    const char* m_file;
    Timestamp m_timestamp; //Log时间类
    LogStream m_logStream; //LogStream类
};

}

}

#endif /* LOGOUT_H */
