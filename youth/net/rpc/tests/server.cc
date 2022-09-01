#include <youth/net/EventLoop.h>
#include <youth/net/HostAddress.hpp>
#include <youth/net/rpc/RpcServer.hpp>
#include <youth/utils/Logging.h>

#include "resolveer.pb.h"
#include "youth/core/Thread.hpp"
#include "youth/utils/ThreadPool.h"

using namespace youth::rpc;
using namespace youth::utils;
using namespace youth::core;
using namespace youth::net;

namespace resolver {

class ResolverServiceImpl : public ResolverService
{
public:
    virtual void Resolve(::google::protobuf::RpcController *controller,
                         const ::resolver::ResolveRequest *request,
                         ::resolver::ResolveResponse *response,
                         ::google::protobuf::Closure *done)
    {
        LOG_INFO << "ResolverServiceImpl::Resolve " << request->address();

        auto addresses = HostAddress::resolve(request->address());
        response->set_resolved(!addresses.empty());
        for (const auto &address : addresses) {
            response->add_ip(address.ip());
            response->add_port(address.port());
        }
        done->Run();
    }
};

} // namespace resolver

int main(int argc, char **argv)
{
    LOG_INFO << "pid = " << getpid();
    EventLoop loop;
    RpcServer server(&loop, HostAddress(65533));
    resolver::ResolverServiceImpl impl;
    server.registerService(&impl);
    server.start();
    loop.loop();
    return 0;
}
