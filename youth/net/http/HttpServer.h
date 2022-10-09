#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <youth/net/Callbacks.h>
#include <youth/net/TcpServer.h>

#include <memory>

namespace youth {

namespace net {
class EventLoop;
class HostAddress;
class Buffer;
} // namespace net

namespace http {

class HttpRequest;
class HttpResponse;
class HttpMethodBuilder;
class HttpMethodFactory;
class HttpContext;

class HttpServer : core::noncopyable
{
public:
    using HttpCallback = std::function<void(const HttpRequest &, HttpResponse *)>;
    using HttpReadyReadCallBack
        = std::function<void(const net::TcpConnectionPtr &, const HttpRequest &)>;

    HttpServer(net::EventLoop *loop,
               const net::HostAddress &listenAddr,
               std::string_view name,
               net::TcpServer::Option option = net::TcpServer::kNoReusePort);

    ~HttpServer();

    net::EventLoop *getLoop() const { return m_server.getLoop(); }

    void setHttpReadyReadCallback(const HttpReadyReadCallBack &callback)
    {
        m_readyReadCallBack = callback;
    }

    void setHttpCallback(const HttpCallback &cb) { m_httpCallback = cb; }

    void registerMethod(std::shared_ptr<HttpMethodFactory> impl);

    void setThreadNum(int numThreads) { m_server.setThreadNum(numThreads); }

    void start();

private:
    void onConnection(const net::TcpConnectionPtr &conn);
    void onMessage(const net::TcpConnectionPtr &conn, net::Buffer *buf, DateTime receiveTime);
    void onReadyRead(const net::TcpConnectionPtr &, HttpContext *context);
    void onRequest(const net::TcpConnectionPtr &, HttpContext *context);

    net::TcpServer m_server;
    HttpCallback m_httpCallback;
    HttpReadyReadCallBack m_readyReadCallBack;
    std::unique_ptr<HttpMethodBuilder> m_methodBuilderPtr;
};

} // namespace http

} // namespace youth

#endif // HTTPSERVER_H
