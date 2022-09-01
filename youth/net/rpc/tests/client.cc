#include <youth/net/EventLoop.h>
#include <youth/net/HostAddress.hpp>
#include <youth/net/rpc/RpcChannel.hpp>
#include <youth/utils/Logging.h>

#include "resolveer.pb.h"
#include "youth/net/TcpClient.h"

using namespace youth::utils;
using namespace youth::core;
using namespace youth::net;
using namespace youth::rpc;

class RpcClient : noncopyable
{
public:
    RpcClient(EventLoop *loop, const HostAddress &serverAddr)
        : m_loop(loop)
        , m_client(m_loop, serverAddr, "RpcClient")
        , m_channel(new RpcChannel)
        , m_stub(m_channel.get())
    {
        m_client.setConnectionCallback(
            std::bind(&RpcClient::onConnection, this, std::placeholders::_1));
        m_client.setMessageCallback(std::bind(&RpcChannel::onMessage,
                                              m_channel.get(),
                                              std::placeholders::_1,
                                              std::placeholders::_2,
                                              std::placeholders::_3));
        m_client.connect();
    }

    ~RpcClient() {}

private:
    void onConnection(const TcpConnectionPtr &conn)
    {
        if (conn->connected()) {
            //channel_.reset(new RpcChannel(conn));
            m_channel->setConnection(conn);
            m_total = 5;
            resolve("baidu.com");
            resolve("tencent.com");
            resolve("alibabagroup.com");
            resolve("github.com");
            resolve("realchuan.github.io");
        } else {
            m_loop->quit();
        }
    }

    void resolve(const std::string &host)
    {
        resolver::ResolveRequest request;
        request.set_address(host);
        resolver::ResolveResponse *response = new resolver::ResolveResponse;

        m_stub.Resolve(NULL,
                       &request,
                       response,
                       NewCallback(this, &RpcClient::resolved, response, host));
    }

    void resolved(resolver::ResolveResponse *resp,
                  std::string host) // pass by value for NewCallback above
    {
        if (resp->resolved()) {
            for (int i = 0; i < resp->ip_size(); i++) {
                LOG_INFO << "resolved " << host << " : " << resp->ip(i) << ":" << resp->port(i);
            }
            LOG_DEBUG << resp->DebugString();
        } else {
            LOG_WARN << "resolved " << host << " failed: " << resp->DebugString();
        }

        if (++m_got >= m_total) {
            m_client.disconnect();
        }
    }

    EventLoop *m_loop;
    TcpClient m_client;
    RpcChannelPtr m_channel;
    resolver::ResolverService::Stub m_stub;
    int m_got = 0;
    int m_total = 0;
};

int main(int argc, char **argv)
{
    LOG_INFO << "pid = " << getpid();
    EventLoop loop;
    RpcClient rpcClient(&loop, HostAddress(65533));
    loop.loop();
    return 0;
}
