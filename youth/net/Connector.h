#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <youth/core/Object.h>

#include "Socket.h"

#include <memory>
#include <functional>

namespace youth {

class Channel;
class EventLoop;
class Connector : noncopyable ,
        public std::enable_shared_from_this<Connector>
{
    typedef std::function<void (int sockfd)> NewConnectionCallback;
public:
    Connector(EventLoop* loop, const TcpAddressInfo& serverAddr);
    ~Connector();

    void setNewConnectionCallback(const NewConnectionCallback& cb);

    void start();  // can be called in any thread
    void restart();  // must be called in loop thread
    void stop();  // can be called in any thread

    const TcpAddressInfo& serverAddress() const;

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

    enum States
    {
        kDisconnected,
        kConnecting,
        kConnected
    };

    static const int kMaxRetryDelayMs = 30*1000;
    static const int kInitRetryDelayMs = 500;

    States m_state = kDisconnected;
    bool m_connect = false; // atomic
    EventLoop *m_eventLoop;
    std::unique_ptr<Channel> m_channelPtr;
    TcpAddressInfo m_serverAddr;
    int m_retryDelayMs = kInitRetryDelayMs;   // 500ms
    NewConnectionCallback m_newConnectionCallback;
};

}

#endif // CONNECTOR_H
