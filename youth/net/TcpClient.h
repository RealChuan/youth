#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <memory>

#include "TcpConnection.h"

#include <youth/core/Mutex.h>

namespace youth
{

using namespace core;

namespace net
{

class Connector;
typedef std::shared_ptr<Connector> ConnectorPtr;

class EventLoop;
class TcpAddressInfo;
class TcpClient : noncopyable
{
public:
    TcpClient(EventLoop* loop,
              const TcpAddressInfo& serverAddr,
              const std::string& nameArg);
    ~TcpClient();

    void connect();
    void disconnect();
    void stop();

    TcpConnectionPtr connection() const;

    EventLoop* getLoop() const;
    bool retry() const;
    void enableRetry();

    const std::string& name() const;

    /// Set connection callback.
    /// Not thread safe.
    void setConnectionCallback(ConnectionCallback cb);

    /// Set message callback.
    /// Not thread safe.
    void setMessageCallback(MessageCallback cb);

    /// Set write complete callback.
    /// Not thread safe.
    void setWriteCompleteCallback(WriteCompleteCallback cb);

private:
    /// Not thread safe, but in loop
    void newConnection(int sockfd);
    /// Not thread safe, but in loop
    void removeConnection(const TcpConnectionPtr& conn);

    EventLoop* m_eventLoop;
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
    TcpConnectionPtr m_connectionPtr GUARDED_BY(m_mutex);
};

}

}

#endif // TCPCLIENT_H
