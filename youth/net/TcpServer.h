#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "Callbacks.h"

#include <atomic>
#include <map>
#include <memory>
#include <string>

namespace youth {

namespace net {

class HostAddress;
class Acceptor;
class EventLoop;
class EventLoopThreadPool;
class TcpServer : noncopyable
{
public:
    using ThreadInitCallback = std::function<void(EventLoop *)>;
    enum Option { kNoReusePort, kReusePort };

    TcpServer(EventLoop *loop,
              const HostAddress &listenAddr,
              const std::string &nameArg,
              Option option = kNoReusePort);
    ~TcpServer(); // force out-line dtor, for std::unique_ptr members.

    const std::string &ipPort() const { return m_ipPort; }
    const std::string &name() const { return m_name; }
    EventLoop *getLoop() const { return m_eventLoop; }

    void setThreadNum(int numThreads);
    void setThreadInitCallback(const ThreadInitCallback &cb) { m_threadInitCallback = cb; }

    std::shared_ptr<EventLoopThreadPool> threadPool() { return m_threadPoolPtr; }

    void start();

    void setConnectionCallback(const ConnectionCallback &cb) { m_connectionCallback = cb; }

    void setMessageCallback(const MessageCallback &cb) { m_messageCallback = cb; }

    void setWriteCompleteCallback(const WriteCompleteCallback &cb) { m_writeCompleteCallback = cb; }

private:
    void newConnection(int sockfd, const HostAddress &peerAddr);
    void removeConnection(const TcpConnectionPtr &conn);
    void removeConnectionInLoop(const TcpConnectionPtr &conn);

    EventLoop *m_eventLoop; // the acceptor loop
    const std::string m_ipPort;
    const std::string m_name;
    std::unique_ptr<Acceptor> m_acceptorPtr; // avoid revealing Acceptor
    std::shared_ptr<EventLoopThreadPool> m_threadPoolPtr;

    ConnectionCallback m_connectionCallback;
    MessageCallback m_messageCallback;
    WriteCompleteCallback m_writeCompleteCallback;
    ThreadInitCallback m_threadInitCallback;

    std::atomic<int> m_started = 0;
    int m_nextConnId = -1;
    std::map<std::string, TcpConnectionPtr> m_connectionMap;
};

} // namespace net

} // namespace youth

#endif // TCPSERVER_H
