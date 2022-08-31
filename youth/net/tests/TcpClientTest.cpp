#include <youth/core/CurrentThread.h>
#include <youth/net/EventLoop.h>
#include <youth/net/HostAddress.hpp>
#include <youth/net/TcpClient.h>
#include <youth/utils/Logging.h>

using namespace youth::core;
using namespace youth::utils;
using namespace youth::net;

class EchoTcpCLient : noncopyable
{
public:
    EchoTcpCLient(EventLoop *loop, const HostAddress &serverAddr)
        : m_eventLoop(loop)
        , m_tcpClientPtr(new TcpClient(loop, serverAddr, "EchoClient"))
    {
        m_tcpClientPtr->setConnectionCallback(
            std::bind(&EchoTcpCLient::connection, this, std::placeholders::_1));
        m_tcpClientPtr->setMessageCallback(std::bind(&EchoTcpCLient::message,
                                                     this,
                                                     std::placeholders::_1,
                                                     std::placeholders::_2,
                                                     std::placeholders::_3));
        m_tcpClientPtr->connect();
    }

private:
    void connection(const TcpConnectionPtr &conn)
    {
        LOG_INFO << conn->localAddress().ipAndPort() << " -> " << conn->peerAddress().ipAndPort()
                 << " is " << (conn->connected() ? "UP" : "DOWN");
        conn->send("Hello Server, I am client\n");
    }

    void message(const TcpConnectionPtr &conn, Buffer *buffer, DateTime timestamp)
    {
        std::string msg(buffer->retrieveAllAsString());
        LOG_INFO << conn->name() << " recv " << msg.size() << " bytes at "
                 << timestamp.toStandardFormat();
        if (msg == "quit\n") {
            conn->send("bye\n");
            conn->shutdown();
            m_eventLoop->runAfter(1, [=] { m_eventLoop->quit(); });
        } else if (msg == "shutdown\n") {
            conn->shutdown();
            m_eventLoop->runAfter(1, [=] { m_eventLoop->quit(); });
        } else {
            conn->send(msg);
        }
    }

    EventLoop *m_eventLoop;
    std::unique_ptr<TcpClient> m_tcpClientPtr;
};

int main(int argc, char **argv)
{
    //Logging::setLogLevel(Logging::DEBUG);

    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
    EventLoop loop;
    EchoTcpCLient echoTcpCLient(&loop, HostAddress(65533));
    loop.loop();
    return 0;
}
