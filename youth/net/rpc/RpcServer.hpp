#pragma once

#include <youth/net/TcpServer.h>

namespace google {
namespace protobuf {

class Service;

} // namespace protobuf
} // namespace google

namespace youth {

namespace rpc {

class RpcServer
{
public:
    RpcServer(net::EventLoop *loop, const net::HostAddress &listenAddr);

    void setThreadNum(int numThreads) { m_server.setThreadNum(numThreads); }

    void registerService(::google::protobuf::Service *);
    void start();

private:
    void onConnection(const net::TcpConnectionPtr &conn);

    // void onMessage(const TcpConnectionPtr& conn,
    //                Buffer* buf,
    //                Timestamp time);

    net::TcpServer m_server;
    std::map<std::string, ::google::protobuf::Service *> m_services;
};

} // namespace rpc

} // namespace youth
