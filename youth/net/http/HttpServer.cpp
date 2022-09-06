#include "HttpServer.h"
#include "HttpContext.h"
#include "HttpMethod.hpp"
#include "HttpRequest.h"
#include "HttpResponse.h"

#include <youth/net/EventLoop.h>
#include <youth/net/HostAddress.hpp>
#include <youth/net/TcpConnection.h>
#include <youth/utils/Logging.h>

#include <any>
#include <type_traits>

namespace youth {

using namespace utils;

namespace http {

HttpServer::HttpServer(net::EventLoop *loop,
                       const net::HostAddress &listenAddr,
                       std::string_view name,
                       net::TcpServer::Option option)
    : m_server(loop, listenAddr, name, option)
    , m_httpCallback()
    , m_methodBuilderPtr(new HttpMethodBuilder)
{
    m_server.setConnectionCallback(
        std::bind(&HttpServer::onConnection, this, std::placeholders::_1));
    m_server.setMessageCallback(std::bind(&HttpServer::onMessage,
                                          this,
                                          std::placeholders::_1,
                                          std::placeholders::_2,
                                          std::placeholders::_3));
}

HttpServer::~HttpServer() {}

void HttpServer::registerMethod(std::shared_ptr<HttpMethodFactory> impl)
{
    m_methodBuilderPtr->registerMethod(impl);
}

void HttpServer::start()
{
    LOG_INFO << "HttpServer[" << m_server.name() << "] starts listenning on " << m_server.ipPort();
    m_server.start();
}

void HttpServer::onConnection(const net::TcpConnectionPtr &conn)
{
    if (conn->connected()) {
        conn->setContext(HttpContext());
    }
}

void HttpServer::onMessage(const net::TcpConnectionPtr &conn, net::Buffer *buf, DateTime receiveTime)
{
    HttpContext *context = std::any_cast<HttpContext>(conn->getMutableContext());

    if (!context->parseRequest(buf, receiveTime)) {
        conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
        conn->shutdown();
    }

    if (context->gotAll()) {
        onRequest(conn, context->request());
        context->reset();
    }
}

void HttpServer::onRequest(const net::TcpConnectionPtr &conn, const HttpRequest &req)
{
    auto connection = req.getHeader("Connection");
    bool close = connection == "close"
                 || (req.getVersion() == HttpRequest::Http10 && connection != "Keep-Alive");
    HttpResponse response(close);
    if (m_httpCallback) {
        m_httpCallback(req, &response);
    } else if (!m_methodBuilderPtr->isEmpty()) {
        m_methodBuilderPtr->onRequest(req, &response);
    } else {
        HttpMethodFactory::defaultCall(req, &response);
    }
    net::Buffer buf;
    response.appendToBuffer(&buf);
    conn->send(&buf);
    if (response.closeConnection()) {
        conn->shutdown();
    }
}

} // namespace http

} // namespace youth
