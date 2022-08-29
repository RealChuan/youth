#ifndef CHANNEL_H
#define CHANNEL_H

#include <youth/core/DateTime.hpp>

#include <functional>
#include <memory>

namespace youth {

using namespace core;

namespace net {

class EventLoop;
class Channel : noncopyable
{
public:
    using EventCallback = std::function<void()>;
    using ReadEventCallback = std::function<void(DateTime)>;

    Channel(EventLoop *loop, int fd);
    ~Channel();

    void handleEvent(DateTime receiveTime);

    void setReadCallback(ReadEventCallback cb) { m_readCallback = std::move(cb); }
    void setWriteCallback(EventCallback cb) { m_writeCallback = std::move(cb); }
    void setCloseCallback(EventCallback cb) { m_closeCallback = std::move(cb); }
    void setErrorCallback(EventCallback cb) { m_errorCallback = std::move(cb); }

    void tie(const std::shared_ptr<void> &obj)
    {
        m_tie = obj;
        m_tied = true;
    }

    void enableReading()
    {
        m_events |= kReadEvent;
        update();
    }
    void disableReading()
    {
        m_events &= ~kReadEvent;
        update();
    }
    void enableWriting()
    {
        m_events |= kWriteEvent;
        update();
    }
    void disableWriting()
    {
        m_events &= ~kWriteEvent;
        update();
    }
    void disableAll()
    {
        m_events = kNoneEvent;
        update();
    }
    bool isWriting() const { return m_events & kWriteEvent; }
    bool isReading() const { return m_events & kReadEvent; }

    bool isNoneEvent() const { return m_events == kNoneEvent; }

    // for EPoll
    int index() const { return m_index; }
    void setIndex(int idx) { m_index = idx; }

    void remove();

    EventLoop *ownerLoop() const { return m_eventLoop; }

    // for debug
    std::string eventsToString() const { return eventsToString(m_fd, m_events); }
    std::string reventsToString() const { return eventsToString(m_fd, m_revents); }

    int fd() const { return m_fd; }
    int events() const { return m_events; }
    void setRevents(int revents) { m_revents = revents; } // used by pollers

private:
    void update();
    void handleEventWithGuard(DateTime receiveTime);

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

} // namespace net

} // namespace youth

#endif // CHANNEL_H
