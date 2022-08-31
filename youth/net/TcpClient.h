#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "TcpConnection.h"

#include <youth/core/Thread.hpp>

#include <memory>

namespace youth {

using namespace core;

namespace net {

class Connector;
using ConnectorPtr = std::shared_ptr<Connector>;

class EventLoop;
class HostAddress;
class TcpClient : noncopyable
{
public:
    TcpClient(EventLoop *loop, const HostAddress &serverAddr, std::string_view name);
    ~TcpClient();

    void connect();
    void disconnect();
    void stop();

    TcpConnectionPtr connection() const
    {
        MutexLock lock(m_mutex);
        return m_tcpConnectionPtr;
    }

    EventLoop *eventLoop() const { return m_eventLoop; }
    bool retry() const { return m_retry; }
    void enableRetry() { m_retry = true; }

    const std::string &name() const { return m_name; }

    /// Not thread safe.
    void setConnectionCallback(ConnectionCallback cb) { m_connectionCallback = std::move(cb); }

    /// Not thread safe.
    void setMessageCallback(MessageCallback cb) { m_messageCallback = std::move(cb); }

    /// Not thread safe.
    void setWriteCompleteCallback(WriteCompleteCallback cb)
    {
        m_writeCompleteCallback = std::move(cb);
    }

private:
    /// Not thread safe, but in loop
    void newConnection(int sockfd);
    /// Not thread safe, but in loop
    void removeConnection(const TcpConnectionPtr &conn);

    EventLoop *m_eventLoop;
    ConnectorPtr m_connectorPtr; // avoid revealing Connector
    const std::string m_name;
    ConnectionCallback m_connectionCallback;
    MessageCallback m_messageCallback;
    WriteCompleteCallback m_writeCompleteCallback;
    bool m_retry;   // atomic
    bool m_connect; // atomic
    // always in loop thread
    int m_nextConnId;
    mutable Mutex m_mutex;
    TcpConnectionPtr m_tcpConnectionPtr;
};

} // namespace net

} // namespace youth

#endif // TCPCLIENT_H
