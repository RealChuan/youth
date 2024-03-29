#include <youth/core/CurrentThread.h>
#include <youth/net/EventLoop.h>
#include <youth/net/HostAddress.hpp>
#include <youth/net/TcpConnection.h>
#include <youth/net/TcpServer.h>
#include <youth/utils/Logging.h>
#include <youth/utils/ThreadPool.h>

using namespace youth::core;
using namespace youth::utils;
using namespace youth::net;

class EchoTcpServer : noncopyable
{
public:
    EchoTcpServer(EventLoop *loop, const HostAddress &localAddr)
        : m_eventLoop(loop)
        , m_tcpServerPtr(new TcpServer(loop, localAddr, "EchoServer"))
    {
        m_tcpServerPtr->setConnectionCallback(
            std::bind(&EchoTcpServer::connection, this, std::placeholders::_1));
        m_tcpServerPtr->setMessageCallback(std::bind(&EchoTcpServer::message,
                                                     this,
                                                     std::placeholders::_1,
                                                     std::placeholders::_2,
                                                     std::placeholders::_3));
    }

    void start(int numThread)
    {
        m_tcpServerPtr->setThreadNum(numThread);
        m_tcpServerPtr->start();
    }

private:
    void connection(const TcpConnectionPtr &conn)
    {
        LOG_INFO << conn->peerAddress().ipAndPort() << " -> " << conn->localAddress().ipAndPort()
                 << " is " << (conn->connected() ? "UP" : "DOWN");
        LOG_INFO << conn->getTcpInfoString();
        conn->send("Hello Client, I am server\n");
        m_eventLoop->runAfter(10, [=] { conn->send("quit\n"); });
    }

    void message(const TcpConnectionPtr &conn, Buffer *buffer, DateTime timestamp)
    {
        std::string msg(buffer->retrieveAllAsString());
        LOG_INFO << conn->name() << " recv " << msg.size() << " bytes at "
                 << timestamp.toStandardFormat();
        if (msg == "quit\n") {
            conn->send("bye\n");
            conn->shutdown();
        } else if (msg == "shutdown\n") {
            conn->shutdown();
            m_eventLoop->runAfter(1, [=] { m_eventLoop->quit(); });
        } else {
            conn->send(msg);
        }
    }

    EventLoop *m_eventLoop;
    std::unique_ptr<TcpServer> m_tcpServerPtr;
};

int main(int argc, char **argv)
{
    //Logging::setLogLevel(Logging::DEBUG);

    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
    EventLoop loop;
    EchoTcpServer echoTcpServer(&loop, HostAddress(65533));
    echoTcpServer.start(ThreadPool::cpuCores() * 2);
    loop.loop();
    return 0;
}
