#pragma once

#include "RpcCodec.hpp"

#include <youth/core/Thread.hpp>
#include <youth/net/Callbacks.h>

#include <google/protobuf/service.h>

#include <atomic>

namespace youth {

namespace rpc {

class RpcChannel : public ::google::protobuf::RpcChannel
{
public:
    RpcChannel();

    explicit RpcChannel(const net::TcpConnectionPtr &conn);

    ~RpcChannel() override;

    void setConnection(const net::TcpConnectionPtr &conn) { m_connectPtr = conn; }

    void setServices(const std::map<std::string, ::google::protobuf::Service *> *services)
    {
        m_services = services;
    }

    // Call the given method of the remote service.  The signature of this
    // procedure looks the same as Service::CallMethod(), but the requirements
    // are less strict in one important way:  the request and response objects
    // need not be of any specific class as long as their descriptors are
    // method->input_type() and method->output_type().
    void CallMethod(const ::google::protobuf::MethodDescriptor *method,
                    ::google::protobuf::RpcController *controller,
                    const ::google::protobuf::Message *request,
                    ::google::protobuf::Message *response,
                    ::google::protobuf::Closure *done) override;

    void onMessage(const net::TcpConnectionPtr &conn, net::Buffer *buf, DateTime receiveTime);

private:
    void onRpcMessage(const net::TcpConnectionPtr &conn,
                      const RpcMessagePtr &messagePtr,
                      DateTime receiveTime);

    void doneCallback(::google::protobuf::Message *response, int64_t id);

    struct OutstandingCall
    {
        ::google::protobuf::Message *response;
        ::google::protobuf::Closure *done;
    };

    RpcCodec m_codec;
    net::TcpConnectionPtr m_connectPtr;
    std::atomic_int64_t m_id;

    Mutex m_mutex;
    std::map<int64_t, OutstandingCall> m_outstandings;

    const std::map<std::string, ::google::protobuf::Service *> *m_services;
};

typedef std::shared_ptr<RpcChannel> RpcChannelPtr;

} // namespace rpc

} // namespace youth
