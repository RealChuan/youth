#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <memory>
#include <any>

#include <youth/core/Timestamp.h>

#include "Socket.h"
#include "Buffer.h"
#include "Callbacks.h"
#include "TcpAddressInfo.h"

namespace youth
{

namespace net
{

class Channel;
class Socket;
class EventLoop;
class TcpAddressInfo;
class TcpConnection: noncopyable,
        public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(EventLoop* loop,
                  const std::string& name,
                  int sockfd,
                  const TcpAddressInfo& localAddr,
                  const TcpAddressInfo& peerAddr);
    ~TcpConnection();

    EventLoop* eventLoop() const;
    const std::string& name() const;
    const TcpAddressInfo& localAddress() const;
    const TcpAddressInfo& peerAddress() const;
    bool connected() const;
    bool disconnected() const;
    // return true if success.
    bool getTcpInfo(struct tcp_info*) const;
    std::string getTcpInfoString() const;

    // void send(string&& message); // C++11
    void send(const void* message, int len);
    void send(const std::string_view& message);
    // void send(Buffer&& message); // C++11
    void send(Buffer* message);  // this one will swap data
    void shutdown(); // NOT thread safe, no simultaneous calling
    // void shutdownAndForceCloseAfter(double seconds); // NOT thread safe, no simultaneous calling
    void forceClose();
    void forceCloseWithDelay(double seconds);
    void setTcpNoDelay(bool on);
    // reading or not
    void startRead();
    void stopRead();
    bool isReading() const;; // NOT thread safe, may race with start/stopReadInLoop

    void setContext(const std::any& context);
    const std::any& getContext() const;

    std::any* getMutableContext();

    void setConnectionCallback(const ConnectionCallback& cb);
    void setMessageCallback(const MessageCallback& cb);
    void setWriteCompleteCallback(const WriteCompleteCallback& cb);
    void setHighWaterMarkCallback(const HighWaterMarkCallback& cb, size_t highWaterMark);

    /// Advanced interface
    Buffer* inputBuffer();
    Buffer* outputBuffer();

    /// Internal use only.
    void setCloseCallback(const CloseCallback& cb);

    // called when TcpServer accepts a new connection
    void connectEstablished();   // should be called only once
    // called when TcpServer has removed me from its map
    void connectDestroyed();  // should be called only once

private:
    void handleRead(Timestamp receiveTime);
    void handleWrite();
    void handleClose();
    void handleError();
    // void sendInLoop(string&& message);
    void sendInLoop(const std::string_view& message);
    void sendInLoop(const void* message, size_t len);
    void shutdownInLoop();
    // void shutdownAndForceCloseInLoop(double seconds);
    void forceCloseInLoop();

    const char* stateToString() const;
    void startReadInLoop();
    void stopReadInLoop();

    enum StateE
    {
        kDisconnected,
        kConnecting,
        kConnected,
        kDisconnecting
    };

    EventLoop* m_eventLoop;
    const std::string m_name;
    StateE m_state = kConnecting;  // FIXME: use atomic variable
    bool m_reading = false;
    // we don't expose those classes to client.
    std::unique_ptr<Socket> m_socketPtr;
    std::unique_ptr<Channel> m_channelPtr;
    const TcpAddressInfo m_localAddr;
    const TcpAddressInfo m_peerAddr;
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

}

}

#endif // TCPCONNECTION_H
