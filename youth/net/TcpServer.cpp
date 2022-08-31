#include "TcpServer.h"
#include "Acceptor.h"
#include "EventLoop.h"
#include "EventLoopThreadPool.h"
#include "HostAddress.hpp"
#include "SocketFunc.h"
#include "TcpConnection.h"

#include <youth/utils/Logging.h>

#include <assert.h>

namespace youth {

using namespace utils;

namespace net {

TcpServer::TcpServer(EventLoop *loop,
                     const HostAddress &listenAddr,
                     std::string_view nameArg,
                     Option option)
    : m_eventLoop(CHECK_NOTNULL(loop))
    , m_ipPort(listenAddr.ipAndPort())
    , m_name(nameArg)
    , m_acceptorPtr(new Acceptor(loop, listenAddr, option == kReusePort))
    , m_threadPoolPtr(new EventLoopThreadPool(loop, m_name))
    , m_connectionCallback(defaultConnectionCallback)
    , m_messageCallback(defaultMessageCallback)
    , m_nextConnId(1)
{
    m_acceptorPtr->setNewConnectionCallback(
        std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2));
}

TcpServer::~TcpServer()
{
    m_eventLoop->assertInLoopThread();
    LOG_DEBUG << "TcpServer::~TcpServer [" << m_name << "] destructing";

    for (auto &item : m_connectionMap) {
        TcpConnectionPtr conn(item.second);
        item.second.reset();
        conn->eventLoop()->runInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
    }
}

void TcpServer::setThreadNum(int numThreads)
{
    assert(0 <= numThreads);
    m_threadPoolPtr->setThreadNum(numThreads);
}

void TcpServer::start()
{
    if (m_started.fetch_add(1) == 0) {
        m_threadPoolPtr->start(m_threadInitCallback);

        assert(!m_acceptorPtr->listenning());
        m_eventLoop->runInLoop(std::bind(&Acceptor::listen, m_acceptorPtr.get()));
    }
}

void TcpServer::newConnection(int sockfd, const HostAddress &peerAddr)
{
    m_eventLoop->assertInLoopThread();
    EventLoop *ioLoop = m_threadPoolPtr->getNextLoop();
    char buf[64];
    snprintf(buf, sizeof buf, "-%s#%d", m_ipPort.c_str(), m_nextConnId);
    ++m_nextConnId;
    std::string connName = m_name + buf;

    LOG_INFO << "TcpServer::newConnection [" << m_name << "] - new connection [" << connName
             << "] from " << peerAddr.ipAndPort();
    HostAddress localAddr(SocketFunc::getLocalAddr(sockfd));
    // FIXME poll with zero timeout to double confirm the new connection
    // FIXME use make_shared if necessary
    TcpConnectionPtr conn(new TcpConnection(ioLoop, connName, sockfd, localAddr, peerAddr));
    m_connectionMap[connName] = conn;
    conn->setConnectionCallback(m_connectionCallback);
    conn->setMessageCallback(m_messageCallback);
    conn->setWriteCompleteCallback(m_writeCompleteCallback);
    conn->setCloseCallback(std::bind(&TcpServer::removeConnection,
                                     this,
                                     std::placeholders::_1)); // FIXME: unsafe
    ioLoop->runInLoop(std::bind(&TcpConnection::connectEstablished, conn));
}

void TcpServer::removeConnection(const TcpConnectionPtr &conn)
{
    // FIXME: unsafe
    m_eventLoop->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr &conn)
{
    m_eventLoop->assertInLoopThread();
    LOG_INFO << "TcpServer::removeConnectionInLoop [" << m_name << "] - connection "
             << conn->name();
    size_t n = m_connectionMap.erase(conn->name());
    (void) n;
    assert(n == 1);
    EventLoop *ioLoop = conn->eventLoop();
    ioLoop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
}

} // namespace net

} // namespace youth
