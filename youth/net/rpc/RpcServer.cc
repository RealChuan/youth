#include "RpcServer.hpp"
#include "RpcChannel.hpp"

#include <youth/net/TcpConnection.h>
#include <youth/utils/Logging.h>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/service.h>

namespace youth {

using namespace utils;

namespace rpc {

RpcServer::RpcServer(net::EventLoop *loop, const net::HostAddress &listenAddr)
    : m_server(loop, listenAddr, "RpcServer")
{
    m_server.setConnectionCallback(std::bind(&RpcServer::onConnection, this, std::placeholders::_1));
    //   server_.setMessageCallback(
    //       std::bind(&RpcServer::onMessage, this, _1, _2, _3));
}

void RpcServer::registerService(google::protobuf::Service *service)
{
    auto *desc = service->GetDescriptor();
    m_services[desc->full_name()] = service;
}

void RpcServer::start()
{
    m_server.start();
}

void RpcServer::onConnection(const net::TcpConnectionPtr &conn)
{
    LOG_INFO << "RpcServer - " << conn->peerAddress().ipAndPort() << " -> "
             << conn->localAddress().ipAndPort() << " is " << (conn->connected() ? "UP" : "DOWN");
    if (conn->connected()) {
        RpcChannelPtr channel(new RpcChannel(conn));
        channel->setServices(&m_services);
        conn->setMessageCallback(std::bind(&RpcChannel::onMessage,
                                           channel.get(),
                                           std::placeholders::_1,
                                           std::placeholders::_2,
                                           std::placeholders::_3));
        conn->setContext(channel);
    } else {
        conn->setContext(RpcChannelPtr());
        // FIXME:
    }
}

// void RpcServer::onMessage(const TcpConnectionPtr& conn,
//                           Buffer* buf,
//                           Timestamp time)
// {
//   RpcChannelPtr& channel = std::any_cast<RpcChannelPtr&>(conn->getContext());
//   channel->onMessage(conn, buf, time);
// }

} // namespace rpc

} // namespace youth
