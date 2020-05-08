#ifndef CHANNEL_H
#define CHANNEL_H

#include <youth/core/Object.h>
#include <youth/core/Timestamp.h>

#include <functional>

namespace youth {

class EventLoop;
class Channel : public noncopyable
{
    typedef std::function<void()> EventCallback;
    typedef std::function<void(Timestamp)> ReadEventCallback;
public:
    Channel(EventLoop* loop, int fd);
    ~Channel();

    void handleEvent(Timestamp receiveTime);

    void setReadCallback(ReadEventCallback cb);
    void setWriteCallback(EventCallback cb);
    void setCloseCallback(EventCallback cb);
    void setErrorCallback(EventCallback cb);

    // for debug
    std::string eventsToString() const;
    std::string reventsToString() const;

    int fd() const;
    void setRevents(int revents); // used by pollers

private:
    void handleEventWithGuard(Timestamp receiveTime);

    static std::string eventsToString(int fd, int events);

    EventLoop* m_loop;
    const int  m_fd;
    int m_events;
    int m_revents; // it's the received event types of epoll or poll
    bool m_logHup;

    ReadEventCallback m_readCallback;
    EventCallback m_writeCallback;
    EventCallback m_closeCallback;
    EventCallback m_errorCallback;
};

}

#endif // CHANNEL_H
