#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <any>
#include <memory>

#include "Buffer.h"
#include "Callbacks.h"
#include "HostAddress.hpp"

struct tcp_info;

namespace youth {

namespace net {

class Channel;
class Socket;
class EventLoop;
class TcpConnection : noncopyable, public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(EventLoop *loop,
                  const std::string &name,
                  int sockfd,
                  const HostAddress &localAddr,
                  const HostAddress &peerAddr);
    ~TcpConnection();

    EventLoop *eventLoop() const { return m_eventLoop; }
    const std::string &name() const { return m_name; }
    const HostAddress &localAddress() const { return m_localAddr; }
    const HostAddress &peerAddress() const { return m_peerAddr; }
    bool connected() const { return m_state == kConnected; }
    bool disconnected() const { return m_state == kDisconnected; }
    // return true if success.
    bool getTcpInfo(struct tcp_info *tcpi) const;
    std::string getTcpInfoString() const;

    //    void send(const std::string &&message)
    //    { send(std::string_view(message.data(), message.size())); }
    void send(const void *data, int len)
    {
        send(std::string_view(static_cast<const char *>(data), len));
    }
    void send(const std::string_view &message);
    // void send(Buffer&& message); // C++11
    void send(Buffer *message); // this one will swap data
    void shutdown();            // NOT thread safe, no simultaneous calling
    // void shutdownAndForceCloseAfter(double seconds); // NOT thread safe, no simultaneous calling
    void forceClose();
    void forceCloseWithDelay(double seconds);
    void setTcpNoDelay(bool on);
    // reading or not
    void startRead();
    void stopRead();
    bool isReading() const
    {
        return m_reading;
    }; // NOT thread safe, may race with start/stopReadInLoop

    void setContext(const std::any &context) { m_context = context; }
    const std::any &getContext() const { return m_context; }
    std::any *getMutableContext() { return &m_context; }

    void setConnectionCallback(const ConnectionCallback &cb) { m_connectionCallback = cb; }
    void setMessageCallback(const MessageCallback &cb) { m_messageCallback = cb; }
    void setWriteCompleteCallback(const WriteCompleteCallback &cb) { m_writeCompleteCallback = cb; }
    void setHighWaterMarkCallback(const HighWaterMarkCallback &cb, size_t highWaterMark)
    {
        m_highWaterMarkCallback = cb;
        m_highWaterMark = highWaterMark;
    }

    /// Advanced interface
    Buffer *inputBuffer() { return &m_inputBuffer; }
    Buffer *outputBuffer() { return &m_outputBuffer; }

    /// Internal use only.
    void setCloseCallback(const CloseCallback &cb) { m_closeCallback = cb; }

    // called when TcpServer accepts a new connection
    void connectEstablished(); // should be called only once
    // called when TcpServer has removed me from its map
    void connectDestroyed(); // should be called only once

private:
    void handleRead(DateTime receiveTime);
    void handleWrite();
    void handleClose();
    void handleError();
    // void sendInLoop(string&& message);
    void sendInLoop(const std::string_view &message) { sendInLoop(message.data(), message.size()); }
    void sendInLoop(const void *message, size_t len);
    void shutdownInLoop();
    // void shutdownAndForceCloseInLoop(double seconds);
    void forceCloseInLoop();

    const char *stateToString() const;
    void startReadInLoop();
    void stopReadInLoop();

    enum StateE { kDisconnected, kConnecting, kConnected, kDisconnecting };

    EventLoop *m_eventLoop;
    const std::string m_name;
    StateE m_state = kConnecting; // FIXME: use atomic variable
    bool m_reading = false;
    // we don't expose those classes to client.
    std::unique_ptr<Socket> m_socketPtr;
    std::unique_ptr<Channel> m_channelPtr;
    const HostAddress m_localAddr;
    const HostAddress m_peerAddr;
    ConnectionCallback m_connectionCallback;
    MessageCallback m_messageCallback;
    WriteCompleteCallback m_writeCompleteCallback;
    HighWaterMarkCallback m_highWaterMarkCallback;
    CloseCallback m_closeCallback;
    size_t m_highWaterMark;
    Buffer m_inputBuffer;
    Buffer m_outputBuffer; // FIXME: use list<Buffer> as output buffer.
    std::any m_context;
    // FIXME: creationTime_, lastReceiveTime_
    //        bytesReceived_, bytesSent_
};

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

} // namespace net

} // namespace youth

#endif // TCPCONNECTION_H
