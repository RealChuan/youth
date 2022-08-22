#ifndef LOGSTREAM_H
#define LOGSTREAM_H

#include <string>

#include "LogBuffer.h"

namespace youth {

namespace utils {

class LogStream : noncopyable
{
public:
    LogStream() {}
    ~LogStream() {}

    const char *GetStreamBuff() { return m_logBuffer.data(); }
    int GetStreamBuffLen() { return m_logBuffer.length(); }

    LogStream &operator<<(bool);
    LogStream &operator<<(char);
    LogStream &operator<<(unsigned char);
    LogStream &operator<<(short);
    LogStream &operator<<(unsigned short);
    LogStream &operator<<(int);
    LogStream &operator<<(unsigned int);
    LogStream &operator<<(long);
    LogStream &operator<<(unsigned long);
    LogStream &operator<<(long long);
    LogStream &operator<<(unsigned long long);
    LogStream &operator<<(float);
    LogStream &operator<<(double);
    LogStream &operator<<(char *pChar);
    LogStream &operator<<(const char *pChar);
    LogStream &operator<<(unsigned char *pChar);
    LogStream &operator<<(const unsigned char *pChar);
    LogStream &operator<<(std::string strString);
    LogStream &operator<<(const void *);

private:
    typedef LogBuffer<normalBuffer> buffer;
    buffer m_logBuffer; //这个就是具体的Buffer
};

} // namespace utils

} // namespace youth

#endif /* LOGSTREAM_H */
