
#ifndef LOGBUFFER_H
#define LOGBUFFER_H

#include <string.h>
#include <algorithm>
#include <stdint.h>

#include <youth/core/Object.h>

namespace youth
{

using namespace core;

namespace utils
{

//下面这个类用来作为一个固定缓存，这个缓存将输出的日志信息
//追加到缓存区中，以便最后一条完整日志的输出。

const int normalBuffer = 4000; //可以使用的正常的缓存大小
const int maxBuffer = 4000 * 1000; //可以使用的最大的缓存大小

template<int SIZE>
class LogBuffer : copyable
{
public:
    LogBuffer()
        : m_dataP(m_data)
    {
        memset(m_data, 0, SIZE);
    }

    ~LogBuffer()
    {
        //nothing
    }

    void reset()
    {
        m_dataP = m_data;
    }

    void bzero()
    {
        memset(m_data, 0, sizeof m_data);
    }

    void append(const char *pData, int Len)
    {
        char *pEndBuffer = &m_data[SIZE - 1];
        if (static_cast<int> (pEndBuffer - m_dataP) > Len)
        {
            memcpy(m_dataP, pData, Len);
            m_dataP += Len;
        }
    }

    //这个方法是用来获取buffer数据
    const char *data()
    {
        return m_data;
    }

    //这个方法是用来获取buffer的长度
    int length()
    {
        return static_cast<int> (m_dataP - m_data);
    }

    //这个方法是用来将某字符串追加到buffer中
    void appendString(const char *pString, int Len)
    {
        memcpy(m_dataP, pString, Len);
        m_dataP += Len;
    }

    //获取剩余可写长度
    int avail() const
    {
        return static_cast<int> (end() - m_dataP);
    }

    //这个方法是用来返回buffer当前的指针位置，
    //以便外部能直接操作buffer，提高效率
    char *current(void)
    {
        return m_dataP;
    }

    //这个方法与current配套使用
    void appendLen(int Len)
    {
        m_dataP += Len;
    }

private:
    const char* end() const
    {
        return m_data + sizeof m_data;
    }

    char m_data[SIZE];
    char *m_dataP;
};

template class LogBuffer<normalBuffer>;
template class LogBuffer<maxBuffer>;

}

}

#endif /* LOGBUFFER_H */
