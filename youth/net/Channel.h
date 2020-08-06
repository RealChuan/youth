#ifndef CHANNEL_H
#define CHANNEL_H

#include <youth/core/Timestamp.h>

#include <functional>
#include <memory>

namespace youth
{

using namespace core;

namespace net
{

class EventLoop;
class Channel : noncopyable
{
    typedef std::function<void()> EventCallback;
    typedef std::function<void(Timestamp)> ReadEventCallback;

public:
    Channel(EventLoop *loop, int fd);
    ~Channel();

    void handleEvent(Timestamp receiveTime);

    void setReadCallback(ReadEventCallback cb);
    void setWriteCallback(EventCallback cb);
    void setCloseCallback(EventCallback cb);
    void setErrorCallback(EventCallback cb);

    void tie(const std::shared_ptr<void>&);

    void enableReading();
    void disableReading();
    void enableWriting();
    void disableWriting();
    void disableAll();
    bool isWriting() const;
    bool isReading() const;

    bool isNoneEvent() const;

    // for EPoll
    int index() const;
    void setIndex(int idx);

    void remove();

    EventLoop *ownerLoop() const;

    // for debug
    std::string eventsToString() const;
    std::string reventsToString() const;

    int fd() const;
    int events() const;
    void setRevents(int revents); // used by pollers

private:
    void update();
    void handleEventWithGuard(Timestamp receiveTime);

    static std::string eventsToString(int fd, int events);

    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    EventLoop *m_eventLoop;

    int m_index; // used by Poller. 索引
    const int m_fd;
    int m_events;
    int m_revents; // it's the received event types of epoll or poll
    bool m_logHup;
    bool m_eventHandling;
    bool m_addedToLoop;
    std::weak_ptr<void> m_tie;
    bool m_tied;

    ReadEventCallback m_readCallback;
    EventCallback m_writeCallback;
    EventCallback m_closeCallback;
    EventCallback m_errorCallback;
};

}

} // namespace youth

#endif // CHANNEL_H
