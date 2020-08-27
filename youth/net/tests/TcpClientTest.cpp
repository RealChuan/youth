#include <youth/net/TcpClient.h>
#include <youth/net/TcpAddressInfo.h>
#include <youth/net/EventLoop.h>
#include <youth/utils/Logging.h>
#include <youth/core/CurrentThread.h>

using namespace youth::core;
using namespace youth::utils;
using namespace youth::net;

class EchoTcpCLient : noncopyable
{
public:
    EchoTcpCLient(EventLoop *loop, const TcpAddressInfo& serverAddr)
        : m_eventLoop(loop)
        , m_TcpClientPtr(new TcpClient(loop, serverAddr, "EchoCLient"))
    {
        m_TcpClientPtr->setConnectionCallback(
                    std::bind(&EchoTcpCLient::connection, this,
                              std::placeholders::_1));
        m_TcpClientPtr->setMessageCallback(
                    std::bind(&EchoTcpCLient::message, this,
                              std::placeholders::_1,
                              std::placeholders::_2,
                              std::placeholders::_3));
        m_TcpClientPtr->connect();
    }

private:
    void connection(const TcpConnectionPtr &conn)
    {
        LOG_INFO << conn->localAddress().ipAndPort() << " -> "
                 << conn->peerAddress().ipAndPort() << " is "
                 << (conn->connected() ? "UP" : "DOWN");
        conn->send("Hello Server, I am client\n");
    }

    void message(const TcpConnectionPtr &conn,
                 Buffer *buffer,
                 Timestamp timestamp)
    {
        std::string msg(buffer->retrieveAllAsString());
        LOG_INFO << conn->name() << " recv " << msg.size()
                 << " bytes at " << timestamp.microSecondsToString();
        if (msg == "quit\n")
        {
            conn->send("bye\n");
            conn->shutdown();
        }
        else if (msg == "shutdown\n")
        {
            conn->shutdown();
            m_eventLoop->runAfter(1, [=]{m_eventLoop->quit();});
        }
        else
        {
            conn->send(msg);
        }
    }

    EventLoop *m_eventLoop;
    std::unique_ptr<TcpClient> m_TcpClientPtr;
};

int main(int argc, char** argv)
{
    //Logging::setLogLevel(Logging::DEBUG);

    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
    EventLoop loop;
    TcpAddressInfo address("127.0.0.1", 65533);
    EchoTcpCLient echoTcpCLient(&loop, address);
    loop.loop();
    return 0;
}
