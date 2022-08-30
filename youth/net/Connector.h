#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "HostAddress.hpp"
#include "Socket.h"

#include <functional>
#include <memory>

namespace youth {

using namespace core;

namespace net {

class Channel;
class EventLoop;
class Connector : noncopyable, public std::enable_shared_from_this<Connector>
{
public:
    using NewConnectionCallback = std::function<void(int sockfd)>;

    Connector(EventLoop *loop, const HostAddress &serverAddr);
    ~Connector();

    void setNewConnectionCallback(const NewConnectionCallback &cb) { m_newConnectionCallback = cb; }

    void start();   // can be called in any thread
    void restart(); // must be called in loop thread
    void stop();    // can be called in any thread

    const HostAddress &serverAddress() const { return m_serverAddr; }

private:
    void startInLoop();
    void stopInLoop();

    void connect();
    void connecting(int sockfd);
    void retry(int sockfd);

    void handleWrite();
    void handleError();

    int removeAndResetChannel();
    void resetChannel();

    enum States { kDisconnected, kConnecting, kConnected };

    static const int kMaxRetryDelayMs = 30 * 1000;
    static const int kInitRetryDelayMs = 500;

    States m_state = kDisconnected;
    bool m_connect = false; // atomic
    EventLoop *m_eventLoop;
    std::unique_ptr<Channel> m_channelPtr;
    HostAddress m_serverAddr;
    int m_retryDelayMs = kInitRetryDelayMs; // 500ms
    NewConnectionCallback m_newConnectionCallback;
};

} // namespace net

} // namespace youth

#endif // CONNECTOR_H
