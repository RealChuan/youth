#include "Connector.h"
#include "EventLoop.h"
#include "SocketFunc.h"
#include "Channel.h"

#include <youth/utils/Logging.h>

#include <assert.h>

namespace youth
{

using namespace utils;

namespace net
{

Connector::Connector(EventLoop *loop, const TcpAddressInfo &serverAddr)
    : m_eventLoop(loop)
    , m_serverAddr(serverAddr)
{
    LOG_DEBUG << "ctor[" << this << "]";
}

Connector::~Connector()
{
    LOG_DEBUG << "dtor[" << this << "]";
    assert(!m_channelPtr);
}

void Connector::setNewConnectionCallback(const Connector::NewConnectionCallback &cb)
{
    m_newConnectionCallback = cb;
}

void Connector::start()
{
    m_connect = true;
    m_eventLoop->runInLoop(std::bind(&Connector::startInLoop, this)); // FIXME: unsafe
}

void Connector::restart()
{
    m_eventLoop->assertInLoopThread();
    m_state = kDisconnected;
    m_retryDelayMs = kInitRetryDelayMs;
    m_connect = true;
    startInLoop();
}

void Connector::stop()
{
    m_connect = false;
    m_eventLoop->queueInLoop(std::bind(&Connector::stopInLoop, this)); // FIXME: unsafe
    // FIXME: cancel timer
}

const TcpAddressInfo &Connector::serverAddress() const
{
    return m_serverAddr;
}

void Connector::startInLoop()
{
    m_eventLoop->assertInLoopThread();
    assert(m_state == kDisconnected);
    if (m_connect)
    {
        connect();
    }
    else
    {
        LOG_DEBUG << "do not connect";
    }
}

void Connector::stopInLoop()
{
    m_eventLoop->assertInLoopThread();
    if (m_state == kConnecting)
    {
        m_state = kDisconnected;
        int sockfd = removeAndResetChannel();
        retry(sockfd);
    }
}

void Connector::connect()
{
    int sockfd = SocketFunc::createNonblockingOrDie(m_serverAddr.family());
    int ret = SocketFunc::connect(sockfd, m_serverAddr.getSockAddr());
    int savedErrno = (ret == 0) ? 0 : errno;
    switch (savedErrno)
    {
    case 0:
    case EINPROGRESS:
    case EINTR:
    case EISCONN:
        connecting(sockfd);
        break;

    case EAGAIN:
    case EADDRINUSE:
    case EADDRNOTAVAIL:
    case ECONNREFUSED:
    case ENETUNREACH:
        retry(sockfd);
        break;

    case EACCES:
    case EPERM:
    case EAFNOSUPPORT:
    case EALREADY:
    case EBADF:
    case EFAULT:
    case ENOTSOCK:
        LOG_ERROR << "connect error in Connector::startInLoop " << savedErrno;
        SocketFunc::close(sockfd);
        break;

    default:
        LOG_ERROR << "Unexpected error in Connector::startInLoop " << savedErrno;
        SocketFunc::close(sockfd);
        // connectErrorCallback_();
        break;
    }
}

void Connector::connecting(int sockfd)
{
    m_state = kConnecting;
    assert(!m_channelPtr);
    m_channelPtr.reset(new Channel(m_eventLoop, sockfd));
    m_channelPtr->setWriteCallback(
                std::bind(&Connector::handleWrite, this)); // FIXME: unsafe
    m_channelPtr->setErrorCallback(
                std::bind(&Connector::handleError, this)); // FIXME: unsafe

    // m_channel->tie(shared_from_this()); is not working,
    // as m_channel is not managed by shared_ptr
    m_channelPtr->enableWriting();
}

void Connector::retry(int sockfd)
{
    SocketFunc::close(sockfd);
    m_state = kDisconnected;
    if (m_connect)
    {
        LOG_INFO << "Connector::retry - Retry connecting to " << m_serverAddr.ipAndPort()
                 << " in " << m_retryDelayMs << " milliseconds. ";
        //        m_eventLoop->runAfter(m_retryDelayMs / 1000.0,
        //                              std::bind(&Connector::startInLoop, shared_from_this()));
        m_retryDelayMs = std::min(m_retryDelayMs * 2, kMaxRetryDelayMs);
    }
    else
    {
        LOG_DEBUG << "do not connect";
    }
}

void Connector::handleWrite()
{
    LOG_DEBUG << "Connector::handleWrite " << m_state;

    if (m_state == kConnecting)
    {
        int sockfd = removeAndResetChannel();
        int err = SocketFunc::getSocketError(sockfd);
        if (err)
        {
            LOG_WARN << "Connector::handleWrite - SO_ERROR = "
                     << err << " " << strerror_tl(err);
            retry(sockfd);
        }
        else if (SocketFunc::isSelfConnect(sockfd))
        {
            LOG_WARN << "Connector::handleWrite - Self connect";
            retry(sockfd);
        }
        else
        {
            m_state = kConnected;
            if (m_state)
            {
                m_newConnectionCallback(sockfd);
            }
            else
            {
                SocketFunc::close(sockfd);
            }
        }
    }
    else
    {
        // what happened?
        assert(m_state == kDisconnected);
    }
}

void Connector::handleError()
{
    LOG_ERROR << "Connector::handleError state=" << m_state;
    if (m_state == kConnecting)
    {
        int sockfd = removeAndResetChannel();
        int err = SocketFunc::getSocketError(sockfd);
        LOG_DEBUG << "SO_ERROR = " << err << " " << strerror_tl(err);
        retry(sockfd);
    }
}

int Connector::removeAndResetChannel()
{
    m_channelPtr->disableAll();
    m_channelPtr->remove();
    int sockfd = m_channelPtr->fd();
    // Can't reset channel_ here, because we are inside Channel::handleEvent
    m_eventLoop->queueInLoop(std::bind(&Connector::resetChannel, this)); // FIXME: unsafe
    return sockfd;
}

void Connector::resetChannel()
{
    m_channelPtr.reset();
}

}

}
