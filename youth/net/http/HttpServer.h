#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <youth/net/Callbacks.h>
#include <youth/net/TcpServer.h>

namespace youth {

namespace net {
class EventLoop;
class HostAddress;
class Buffer;
} // namespace net

namespace http {

class HttpRequest;
class HttpResponse;

class HttpServer : core::noncopyable
{
public:
    typedef std::function<void(const HttpRequest &, HttpResponse *)> HttpCallback;

    HttpServer(net::EventLoop *loop,
               const net::HostAddress &listenAddr,
               std::string_view name,
               net::TcpServer::Option option = net::TcpServer::kNoReusePort);

    ~HttpServer() {}

    net::EventLoop *getLoop() const { return m_server.getLoop(); }

    void setHttpCallback(const HttpCallback &cb) { m_httpCallback = cb; }

    void setThreadNum(int numThreads) { m_server.setThreadNum(numThreads); }

    void start();

private:
    void onConnection(const net::TcpConnectionPtr &conn);
    void onMessage(const net::TcpConnectionPtr &conn, net::Buffer *buf, DateTime receiveTime);
    void onRequest(const net::TcpConnectionPtr &, const HttpRequest &);

    net::TcpServer m_server;
    HttpCallback m_httpCallback;
};

} // namespace http

} // namespace youth

#endif // HTTPSERVER_H
