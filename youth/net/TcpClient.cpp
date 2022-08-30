#include "TcpClient.h"
#include "Connector.h"
#include "EventLoop.h"
#include "SocketFunc.h"

#include <youth/utils/Logging.h>

#include <assert.h>

namespace youth {

using namespace utils;

namespace net {

void removeConnection(EventLoop *loop, const TcpConnectionPtr &conn)
{
    loop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
}

void removeConnector(const ConnectorPtr &connector)
{
    // connector->stop();
}

TcpClient::TcpClient(EventLoop *loop, const HostAddress &serverAddr, const std::string &name)
    : m_eventLoop(CHECK_NOTNULL(loop))
    , m_connectorPtr(new Connector(loop, serverAddr))
    , m_name(name)
    , m_connectionCallback(defaultConnectionCallback)
    , m_messageCallback(defaultMessageCallback)
    , m_retry(false)
    , m_connect(true)
    , m_nextConnId(1)
{
    m_connectorPtr->setNewConnectionCallback(
        std::bind(&TcpClient::newConnection, this, std::placeholders::_1));
    // FIXME setConnectFailedCallback
    LOG_INFO << "TcpClient::TcpClient[" << m_name << "] - connector " << m_connectorPtr.get();
}

TcpClient::~TcpClient()
{
    LOG_INFO << "TcpClient::~TcpClient[" << m_name << "] - connector " << m_connectorPtr.get();
    TcpConnectionPtr conn;
    bool unique = false;
    {
        MutexLock lock(m_mutex);
        unique = m_tcpConnectionPtr.unique();
        conn = m_tcpConnectionPtr;
    }
    if (conn) {
        assert(m_eventLoop == conn->eventLoop());
        // FIXME: not 100% safe, if we are in different thread
        CloseCallback cb = std::bind(&net::removeConnection, m_eventLoop, std::placeholders::_1);
        m_eventLoop->runInLoop(std::bind(&TcpConnection::setCloseCallback, conn, cb));
        if (unique) {
            conn->forceClose();
        }
    } else {
        m_connectorPtr->stop();
        // FIXME: HACK
        m_eventLoop->runAfter(1, std::bind(&net::removeConnector, m_connectorPtr));
    }
}

void TcpClient::connect()
{
    // FIXME: check state
    LOG_INFO << "TcpClient::connect[" << m_name << "] - connecting to "
             << m_connectorPtr->serverAddress().ipAndPort();
    m_connect = true;
    m_connectorPtr->start();
}

void TcpClient::disconnect()
{
    m_connect = false;

    {
        MutexLock lock(m_mutex);
        if (m_tcpConnectionPtr) {
            m_tcpConnectionPtr->shutdown();
        }
    }
}

void TcpClient::stop()
{
    m_connect = false;
    m_connectorPtr->stop();
}

void TcpClient::newConnection(int sockfd)
{
    m_eventLoop->assertInLoopThread();
    HostAddress peerAddr(SocketFunc::getPeerAddr(sockfd));
    char buf[32];
    snprintf(buf, sizeof buf, ":%s#%d", peerAddr.ipAndPort().c_str(), m_nextConnId);
    ++m_nextConnId;
    std::string connName = m_name + buf;

    HostAddress localAddr(SocketFunc::getLocalAddr(sockfd));
    // FIXME poll with zero timeout to double confirm the new connection
    // FIXME use make_shared if necessary
    TcpConnectionPtr conn(new TcpConnection(m_eventLoop, connName, sockfd, localAddr, peerAddr));

    conn->setConnectionCallback(m_connectionCallback);
    conn->setMessageCallback(m_messageCallback);
    conn->setWriteCompleteCallback(m_writeCompleteCallback);
    conn->setCloseCallback(std::bind(&TcpClient::removeConnection,
                                     this,
                                     std::placeholders::_1)); // FIXME: unsafe
    {
        MutexLock lock(m_mutex);
        m_tcpConnectionPtr = conn;
    }
    conn->connectEstablished();
}

void TcpClient::removeConnection(const TcpConnectionPtr &conn)
{
    m_eventLoop->assertInLoopThread();
    assert(m_eventLoop == conn->eventLoop());

    {
        MutexLock lock(m_mutex);
        assert(m_tcpConnectionPtr == conn);
        m_tcpConnectionPtr.reset();
    }

    m_eventLoop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
    if (m_retry && m_connect) {
        LOG_INFO << "TcpClient::connect[" << m_name << "] - Reconnecting to "
                 << m_connectorPtr->serverAddress().ipAndPort();
        m_connectorPtr->restart();
    }
}

} // namespace net

} // namespace youth
