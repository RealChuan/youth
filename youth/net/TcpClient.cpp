#include "TcpClient.h"
#include "Connector.h"
#include "EventLoop.h"
#include "SocketFunc.h"

#include <youth/utils/Logging.h>

#include <assert.h>

namespace youth
{

using namespace utils;

namespace net
{

void removeConnection(EventLoop* loop, const TcpConnectionPtr& conn)
{
    loop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
}

void removeConnector(const ConnectorPtr& connector)
{
    //connector->
}

TcpClient::TcpClient(EventLoop *loop,
                     const TcpAddressInfo &serverAddr,
                     const std::string &nameArg)
    : loop_(CHECK_NOTNULL(loop)),
      connector_(new Connector(loop, serverAddr)),
      name_(nameArg),
      connectionCallback_(defaultConnectionCallback),
      messageCallback_(defaultMessageCallback),
      retry_(false),
      connect_(true),
      nextConnId_(1)
{
    connector_->setNewConnectionCallback(
                std::bind(&TcpClient::newConnection, this, std::placeholders::_1));
    // FIXME setConnectFailedCallback
    LOG_INFO << "TcpClient::TcpClient[" << name_
             << "] - connector " << connector_.get();

}

TcpClient::~TcpClient()
{
    LOG_INFO << "TcpClient::~TcpClient[" << name_
             << "] - connector " << connector_.get();
    TcpConnectionPtr conn;
    bool unique = false;
    {
        MutexLock lock(mutex_);
        unique = connection_.unique();
        conn = connection_;
    }
    if (conn)
    {
        assert(loop_ == conn->eventLoop());
        // FIXME: not 100% safe, if we are in different thread
        CloseCallback cb = std::bind(&net::removeConnection, loop_, std::placeholders::_1);
        loop_->runInLoop(
                    std::bind(&TcpConnection::setCloseCallback, conn, cb));
        if (unique)
        {
            conn->forceClose();
        }
    }
    else
    {
        connector_->stop();
        // FIXME: HACK
        //loop_->runAfter(1, std::bind(&net::removeConnector, connector_));
    }
}

void TcpClient::connect()
{
    // FIXME: check state
    LOG_INFO << "TcpClient::connect[" << name_ << "] - connecting to "
             << connector_->serverAddress().ipAndPort();
    connect_ = true;
    connector_->start();
}

void TcpClient::disconnect()
{
    connect_ = false;

    {
        MutexLock lock(mutex_);
        if (connection_)
        {
            connection_->shutdown();
        }
    }
}

void TcpClient::stop()
{
    connect_ = false;
    connector_->stop();
}

TcpConnectionPtr TcpClient::connection() const
{
    MutexLock lock(mutex_);
    return connection_;
}

EventLoop *TcpClient::getLoop() const
{
    return loop_;
}

bool TcpClient::retry() const
{
    return retry_;
}

void TcpClient::enableRetry()
{
    retry_ = true;
}

const std::string &TcpClient::name() const
{
    return name_;
}

void TcpClient::setConnectionCallback(ConnectionCallback cb)
{
    connectionCallback_ = std::move(cb);
}

void TcpClient::setMessageCallback(MessageCallback cb)
{
    messageCallback_ = std::move(cb);
}

void TcpClient::setWriteCompleteCallback(WriteCompleteCallback cb)
{
    writeCompleteCallback_ = std::move(cb);
}

void TcpClient::newConnection(int sockfd)
{
    loop_->assertInLoopThread();
    TcpAddressInfo peerAddr(SocketFunc::getPeerAddr(sockfd));
    char buf[32];
    snprintf(buf, sizeof buf, ":%s#%d", peerAddr.ipAndPort().c_str(), nextConnId_);
    ++nextConnId_;
    std::string connName = name_ + buf;

    TcpAddressInfo localAddr(SocketFunc::getLocalAddr(sockfd));
    // FIXME poll with zero timeout to double confirm the new connection
    // FIXME use make_shared if necessary
    TcpConnectionPtr conn(new TcpConnection(loop_,
                                            connName,
                                            sockfd,
                                            localAddr,
                                            peerAddr));

    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setWriteCompleteCallback(writeCompleteCallback_);
    conn->setCloseCallback(
                std::bind(&TcpClient::removeConnection, this, std::placeholders::_1)); // FIXME: unsafe
    {
        MutexLock lock(mutex_);
        connection_ = conn;
    }
    conn->connectEstablished();
}

void TcpClient::removeConnection(const TcpConnectionPtr &conn)
{
    loop_->assertInLoopThread();
    assert(loop_ == conn->eventLoop());

    {
        MutexLock lock(mutex_);
        assert(connection_ == conn);
        connection_.reset();
    }

    loop_->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
    if (retry_ && connect_)
    {
        LOG_INFO << "TcpClient::connect[" << name_ << "] - Reconnecting to "
                 << connector_->serverAddress().ipAndPort();
        connector_->restart();
    }
}

}

}
