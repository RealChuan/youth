#include "RpcChannel.hpp"

#include <youth/net/rpc/rpc.pb.h>
#include <youth/utils/Logging.h>

#include <google/protobuf/message.h>

namespace youth {

using namespace utils;

namespace rpc {

RpcChannel::RpcChannel()
    : m_codec(std::bind(&RpcChannel::onRpcMessage,
                        this,
                        std::placeholders::_1,
                        std::placeholders::_2,
                        std::placeholders::_3))
    , m_services(NULL)
{
    LOG_INFO << "RpcChannel::ctor - " << this;
}

RpcChannel::RpcChannel(const net::TcpConnectionPtr &conn)
    : m_codec(std::bind(&RpcChannel::onRpcMessage,
                        this,
                        std::placeholders::_1,
                        std::placeholders::_2,
                        std::placeholders::_3))
    , m_connectPtr(conn)
    , m_services(NULL)
{
    LOG_INFO << "RpcChannel::ctor - " << this;
}

RpcChannel::~RpcChannel()
{
    LOG_INFO << "RpcChannel::dtor - " << this;
    for (const auto &outstanding : m_outstandings) {
        auto out = outstanding.second;
        delete out.response;
        delete out.done;
    }
}

// Call the given method of the remote service.  The signature of this
// procedure looks the same as Service::CallMethod(), but the requirements
// are less strict in one important way:  the request and response objects
// need not be of any specific class as long as their descriptors are
// method->input_type() and method->output_type().
void RpcChannel::CallMethod(const ::google::protobuf::MethodDescriptor *method,
                            google::protobuf::RpcController *controller,
                            const ::google::protobuf::Message *request,
                            ::google::protobuf::Message *response,
                            ::google::protobuf::Closure *done)
{
    RpcMessage message;
    message.set_type(REQUEST);
    int64_t id = m_id.fetch_add(1);
    message.set_id(id);
    message.set_service(method->service()->full_name());
    message.set_method(method->name());
    message.set_request(request->SerializeAsString()); // FIXME: error check

    OutstandingCall out = {response, done};
    {
        MutexLock lock(m_mutex);
        m_outstandings[id] = out;
    }
    m_codec.send(m_connectPtr, message);
}

void RpcChannel::onMessage(const net::TcpConnectionPtr &conn, net::Buffer *buf, DateTime receiveTime)
{
    m_codec.onMessage(conn, buf, receiveTime);
}

void RpcChannel::onRpcMessage(const net::TcpConnectionPtr &conn,
                              const RpcMessagePtr &messagePtr,
                              DateTime receiveTime)
{
    assert(conn == conn_);
    //printf("%s\n", message.DebugString().c_str());
    RpcMessage &message = *messagePtr;
    if (message.type() == RESPONSE) {
        int64_t id = message.id();
        assert(message.has_response() || message.has_error());

        OutstandingCall out = {NULL, NULL};

        {
            MutexLock lock(m_mutex);
            auto it = m_outstandings.find(id);
            if (it != m_outstandings.end()) {
                out = it->second;
                m_outstandings.erase(it);
            }
        }

        if (out.response) {
            std::unique_ptr<google::protobuf::Message> d(out.response);
            if (message.has_response()) {
                out.response->ParseFromString(message.response());
            }
            if (out.done) {
                out.done->Run();
            }
        }
    } else if (message.type() == REQUEST) {
        // FIXME: extract to a function
        auto error = WRONG_PROTO;
        if (m_services) {
            auto it = m_services->find(message.service());
            if (it != m_services->end()) {
                auto service = it->second;
                assert(service != NULL);
                auto desc = service->GetDescriptor();
                auto method = desc->FindMethodByName(message.method());
                if (method) {
                    std::unique_ptr<google::protobuf::Message> request(
                        service->GetRequestPrototype(method).New());
                    if (request->ParseFromString(message.request())) {
                        auto response = service->GetResponsePrototype(method).New();
                        // response is deleted in doneCallback
                        int64_t id = message.id();
                        service->CallMethod(method,
                                            NULL,
                                            request.get(),
                                            response,
                                            NewCallback(this,
                                                        &RpcChannel::doneCallback,
                                                        response,
                                                        id));
                        error = NO_ERROR;
                    } else {
                        error = INVALID_REQUEST;
                    }
                } else {
                    error = NO_METHOD;
                }
            } else {
                error = NO_SERVICE;
            }
        } else {
            error = NO_SERVICE;
        }
        if (error != NO_ERROR) {
            RpcMessage response;
            response.set_type(RESPONSE);
            response.set_id(message.id());
            response.set_error(error);
            m_codec.send(m_connectPtr, response);
        }
    } else if (message.type() == ERROR) {
    }
}

void RpcChannel::doneCallback(::google::protobuf::Message *response, int64_t id)
{
    std::unique_ptr<google::protobuf::Message> d(response);
    RpcMessage message;
    message.set_type(RESPONSE);
    message.set_id(id);
    message.set_response(response->SerializeAsString()); // FIXME: error check
    m_codec.send(m_connectPtr, message);
}

} // namespace rpc

} // namespace youth
