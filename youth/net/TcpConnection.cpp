#include "TcpConnection.h"
#include "Channel.h"
#include "EventLoop.h"
#include "SocketFunc.h"

#include <youth/utils/Logging.h>
#include <youth/core/WeakCallback.h>

#include <assert.h>

namespace youth
{

using namespace utils;

namespace net
{

void defaultConnectionCallback(const TcpConnectionPtr& conn)
{
    LOG_DEBUG << conn->localAddress().ipAndPort() << " -> "
              << conn->peerAddress().ipAndPort() << " is "
              << (conn->connected() ? "UP" : "DOWN");
    // do not call conn->forceClose(), because some users want to register message callback only.
}

void defaultMessageCallback(const TcpConnectionPtr&,
                            Buffer* buf,
                            Timestamp)
{
    buf->retrieveAll();
}

TcpConnection::TcpConnection(EventLoop *loop,
                             const std::string &name,
                             int sockfd,
                             const TcpAddressInfo &localAddr,
                             const TcpAddressInfo &peerAddr)
    : m_eventLoop(CHECK_NOTNULL(loop))
    , m_name(name)
    , m_state(kConnecting)
    , m_reading(true)
    , m_socketPtr(new Socket(sockfd))
    , m_channelPtr(new Channel(loop, sockfd))
    , m_localAddr(localAddr)
    , m_peerAddr(peerAddr)
    , m_highWaterMark(64*1024*1024)
{
    m_channelPtr->setReadCallback(
                std::bind(&TcpConnection::handleRead, this, std::placeholders::_1));
    m_channelPtr->setWriteCallback(
                std::bind(&TcpConnection::handleWrite, this));
    m_channelPtr->setCloseCallback(
                std::bind(&TcpConnection::handleClose, this));
    m_channelPtr->setErrorCallback(
                std::bind(&TcpConnection::handleError, this));
    LOG_DEBUG << "TcpConnection::ctor[" <<  m_name << "] at " << this
              << " fd=" << sockfd;
    m_socketPtr->setKeepAlive(true);
}

TcpConnection::~TcpConnection()
{
    LOG_DEBUG << "TcpConnection::dtor[" <<  m_name << "] at " << this
              << " fd=" << m_channelPtr->fd()
              << " state=" << stateToString();
    assert(m_state == kDisconnected);
}

EventLoop *TcpConnection::eventLoop() const
{
    return m_eventLoop;
}

const std::string& TcpConnection::name() const
{
    return m_name;
}

const TcpAddressInfo &TcpConnection::localAddress() const
{
    return m_localAddr;
}

const TcpAddressInfo &TcpConnection::peerAddress() const
{
    return m_peerAddr;
}

bool TcpConnection::connected() const
{
    return m_state == kConnected;
}

bool TcpConnection::disconnected() const
{
    return m_state == kDisconnected;
}

bool TcpConnection::getTcpInfo(tcp_info *tcpi) const
{
    return m_socketPtr->getTcpInfo(tcpi);
}

std::string TcpConnection::getTcpInfoString() const
{
    char buf[1024];
    buf[0] = '\0';
    m_socketPtr->getTcpInfoString(buf, sizeof buf);
    return buf;
}

void TcpConnection::send(const void *data, int len)
{
    send(std::string_view(static_cast<const char*>(data), len));
}

void TcpConnection::send(const std::string_view &message)
{
    if (m_state == kConnected)
    {
        if (m_eventLoop->isInLoopThread())
        {
            sendInLoop(message);
        }
        else
        {
            void (TcpConnection::*fp)(const std::string_view& message) = &TcpConnection::sendInLoop;
            m_eventLoop->runInLoop(std::bind(fp, this, std::string(message)));
            //std::forward<string>(message)));
        }
    }
}

void TcpConnection::send(Buffer *buf)
{
    if (m_state == kConnected)
    {
        if (m_eventLoop->isInLoopThread())
        {
            sendInLoop(buf->peek(), buf->readableBytes());
            buf->retrieveAll();
        }
        else
        {
            void (TcpConnection::*fp)(const std::string_view& message) = &TcpConnection::sendInLoop;
            m_eventLoop->runInLoop(
                        std::bind(fp,
                                  this,     // FIXME
                                  buf->retrieveAllAsString()));
            //std::forward<string>(message)));
        }
    }
}

void TcpConnection::shutdown()
{
    // FIXME: use compare and swap
    if (m_state == kConnected)
    {
        m_state = kDisconnecting;
        // FIXME: shared_from_this()?
        m_eventLoop->runInLoop(std::bind(&TcpConnection::shutdownInLoop, this));
    }
}

void TcpConnection::forceClose()
{
    // FIXME: use compare and swap
    if (m_state == kConnected || m_state == kDisconnecting)
    {
        m_state = kDisconnecting;
        m_eventLoop->queueInLoop(std::bind(&TcpConnection::forceCloseInLoop,
                                           shared_from_this()));
    }
}

void TcpConnection::forceCloseWithDelay(double seconds)
{
    if (m_state == kConnected || m_state == kDisconnecting)
    {
        m_state = kDisconnecting;
        m_eventLoop->runAfter(seconds,
                              makeWeakCallback(shared_from_this(),
                                               &TcpConnection::forceClose));  // not forceCloseInLoop to avoid race condition
    }
}

void TcpConnection::setTcpNoDelay(bool on)
{
    m_socketPtr->setTcpNoDelay(on);
}

void TcpConnection::startRead()
{
    m_eventLoop->runInLoop(std::bind(&TcpConnection::startReadInLoop, this));
}

void TcpConnection::stopRead()
{
    m_eventLoop->runInLoop(std::bind(&TcpConnection::stopReadInLoop, this));
}

bool TcpConnection::isReading() const
{
    return m_reading;
}

void TcpConnection::setContext(const std::any &context)
{
    m_context = context;
}

const std::any &TcpConnection::getContext() const
{
    return m_context;
}

std::any *TcpConnection::getMutableContext()
{
    return &m_context;
}

void TcpConnection::setConnectionCallback(const ConnectionCallback &cb)
{
    m_connectionCallback = cb;
}

void TcpConnection::setMessageCallback(const MessageCallback &cb)
{
    m_messageCallback = cb;
}

void TcpConnection::setWriteCompleteCallback(const WriteCompleteCallback &cb)
{
    m_writeCompleteCallback = cb;
}

void TcpConnection::setHighWaterMarkCallback(const HighWaterMarkCallback &cb,
                                             size_t highWaterMark)
{
    m_highWaterMarkCallback = cb;
    m_highWaterMark = highWaterMark;
}

Buffer *TcpConnection::inputBuffer()
{
    return &m_inputBuffer;
}

Buffer *TcpConnection::outputBuffer()
{
    return &m_outputBuffer;
}

void TcpConnection::setCloseCallback(const CloseCallback &cb)
{
    m_closeCallback = cb;
}

void TcpConnection::connectEstablished()
{
    m_eventLoop->assertInLoopThread();
    assert(m_state == kConnecting);
    m_state = kConnected;
    m_channelPtr->tie(shared_from_this());
    m_channelPtr->enableReading();

    m_connectionCallback(shared_from_this());
}

void TcpConnection::connectDestroyed()
{
    m_eventLoop->assertInLoopThread();
    if (m_state == kConnected)
    {
        m_state = kDisconnected;
        m_channelPtr->disableAll();

        m_connectionCallback(shared_from_this());
    }
    m_channelPtr->remove();
}

void TcpConnection::handleRead(Timestamp receiveTime)
{
    m_eventLoop->assertInLoopThread();
    int savedErrno = 0;
    ssize_t n = m_inputBuffer.readFd(m_channelPtr->fd(), &savedErrno);
    if (n > 0)
    {
        m_messageCallback(shared_from_this(), &m_inputBuffer, receiveTime);
    }
    else if (n == 0)
    {
        handleClose();
    }
    else
    {
        errno = savedErrno;
        LOG_ERROR << "TcpConnection::handleRead";
        handleError();
    }
}

void TcpConnection::handleWrite()
{
    m_eventLoop->assertInLoopThread();
    if (m_channelPtr->isWriting())
    {
        ssize_t n = SocketFunc::write(m_channelPtr->fd(),
                                      m_outputBuffer.peek(),
                                      m_outputBuffer.readableBytes());
        if (n > 0)
        {
            m_outputBuffer.retrieve(n);
            if (m_outputBuffer.readableBytes() == 0)
            {
                m_channelPtr->disableWriting();
                if (m_writeCompleteCallback)
                {
                    m_eventLoop->queueInLoop(std::bind(m_writeCompleteCallback,
                                                       shared_from_this()));
                }
                if (m_state == kDisconnecting)
                {
                    shutdownInLoop();
                }
            }
        }
        else
        {
            LOG_ERROR << "TcpConnection::handleWrite";
            // if (state_ == kDisconnecting)
            // {
            //   shutdownInLoop();
            // }
        }
    }
    else
    {
        LOG_DEBUG << "Connection fd = " << m_channelPtr->fd()
                  << " is down, no more writing";
    }
}

void TcpConnection::handleClose()
{
    m_eventLoop->assertInLoopThread();
    LOG_DEBUG << "fd = " << m_channelPtr->fd() << " state = " << stateToString();
    assert(m_state == kConnected || m_state == kDisconnecting);
    // we don't close fd, leave it to dtor, so we can find leaks easily.
    m_state = kDisconnected;
    m_channelPtr->disableAll();

    TcpConnectionPtr guardThis(shared_from_this());
    m_connectionCallback(guardThis);
    // must be the last line
    m_closeCallback(guardThis);
}

void TcpConnection::handleError()
{
    int err = SocketFunc::getSocketError(m_channelPtr->fd());
    LOG_ERROR << "TcpConnection::handleError [" << m_name
              << "] - SO_ERROR = " << err << " " << strerror_tl(err);
}

void TcpConnection::sendInLoop(const std::string_view &message)
{
    sendInLoop(message.data(), message.size());
}

void TcpConnection::sendInLoop(const void *data, size_t len)
{
    m_eventLoop->assertInLoopThread();
    ssize_t nwrote = 0;
    size_t remaining = len;
    bool faultError = false;
    if (m_state == kDisconnected)
    {
        LOG_WARN << "disconnected, give up writing";
        return;
    }
    // if no thing in output queue, try writing directly
    if (!m_channelPtr->isWriting() && m_outputBuffer.readableBytes() == 0)
    {
        nwrote = SocketFunc::write(m_channelPtr->fd(), data, len);
        if (nwrote >= 0)
        {
            remaining = len - nwrote;
            if (remaining == 0 && m_writeCompleteCallback)
            {
                m_eventLoop->queueInLoop(std::bind(m_writeCompleteCallback,
                                                   shared_from_this()));
            }
        }
        else // nwrote < 0
        {
            nwrote = 0;
            if (errno != EWOULDBLOCK)
            {
                LOG_ERROR << "TcpConnection::sendInLoop";
                if (errno == EPIPE || errno == ECONNRESET) // FIXME: any others?
                {
                    faultError = true;
                }
            }
        }
    }

    assert(remaining <= len);
    if (!faultError && remaining > 0)
    {
        size_t oldLen = m_outputBuffer.readableBytes();
        if (oldLen + remaining >= m_highWaterMark
                && oldLen < m_highWaterMark
                && m_highWaterMarkCallback)
        {
            m_eventLoop->queueInLoop(std::bind(m_highWaterMarkCallback, shared_from_this(), oldLen + remaining));
        }
        m_outputBuffer.append(static_cast<const char*>(data)+nwrote, remaining);
        if (!m_channelPtr->isWriting())
        {
            m_channelPtr->enableWriting();
        }
    }
}

void TcpConnection::shutdownInLoop()
{
    m_eventLoop->assertInLoopThread();
    if (!m_channelPtr->isWriting())
    {
        // we are not writing
        m_socketPtr->shutdownWrite();
    }
}

void TcpConnection::forceCloseInLoop()
{
    m_eventLoop->assertInLoopThread();
    if (m_state == kConnected || m_state == kDisconnecting)
    {
        // as if we received 0 byte in handleRead();
        handleClose();
    }
}

const char *TcpConnection::stateToString() const
{
    switch (m_state)
    {
    case kDisconnected: return "kDisconnected";
    case kConnecting: return "kConnecting";
    case kConnected: return "kConnected";
    case kDisconnecting: return "kDisconnecting";
    default: return "unknown state";
    }
}

void TcpConnection::startReadInLoop()
{
    m_eventLoop->assertInLoopThread();
    if (!m_reading || !m_channelPtr->isReading())
    {
        m_channelPtr->enableReading();
        m_reading = true;
    }
}

void TcpConnection::stopReadInLoop()
{
    m_eventLoop->runInLoop(std::bind(&TcpConnection::stopReadInLoop, this));
}

}

}
