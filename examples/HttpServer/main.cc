#include "GetMethod.hpp"
#include "HeadMethod.hpp"
#include "PostMethod.hpp"

#include <youth/net/EventLoop.h>
#include <youth/net/HostAddress.hpp>
#include <youth/net/http/HttpServer.h>
#include <youth/utils/ThreadPool.h>

using namespace youth::net;
using namespace youth::utils;

int main()
{
    EventLoop loop;
    HttpServer server(&loop, HostAddress(8000), "Test");
    std::shared_ptr<GetMethod> getImpl(new GetMethod);
    std::shared_ptr<HeadMethod> headImpl(new HeadMethod);
    std::shared_ptr<PostMethod> postImpl(new PostMethod);
    server.registerMethod(getImpl);
    server.registerMethod(headImpl);
    server.registerMethod(postImpl);
    server.setThreadNum(ThreadPool::cpuCores() * 2);
    server.start();
    loop.loop();
    return 0;
}
