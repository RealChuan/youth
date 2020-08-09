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

    EventLoop* loop_;
    ConnectorPtr connector_; // avoid revealing Connector
    const std::string name_;
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    bool retry_;   // atomic
    bool connect_; // atomic
    // always in loop thread
    int nextConnId_;
    mutable Mutex mutex_;
    TcpConnectionPtr connection_ GUARDED_BY(mutex_);
};

}

}

#endif // TCPCLIENT_H
