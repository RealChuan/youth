#include "HttpServer.h"
#include "HttpResponse.h"
#include "HttpRequest.h"
#include "HttpContext.h"

#include <youth/net/TcpConnection.h>
#include <youth/net/EventLoop.h>
#include <youth/net/TcpAddressInfo.h>
#include <youth/utils/Logging.h>

#include <any>

namespace youth
{

    using namespace utils;

    namespace http
    {

        void defaultHttpCallback(const HttpRequest &, HttpResponse *resp)
        {
            resp->setStatusCode(HttpResponse::k404NotFound);
            resp->setStatusMessage("Not Found");
            resp->setCloseConnection(true);
        }

        HttpServer::HttpServer(net::EventLoop *loop,
                               const net::TcpAddressInfo &listenAddr,
                               const std::string &name,
                               net::TcpServer::Option option)
            : m_server(loop, listenAddr, name, option), m_httpCallback(defaultHttpCallback)
        {
            m_server.setConnectionCallback(
                std::bind(&HttpServer::onConnection, this, std::placeholders::_1));
            m_server.setMessageCallback(
                std::bind(&HttpServer::onMessage, this, std::placeholders::_1,
                          std::placeholders::_2,
                          std::placeholders::_3));
        }

        void HttpServer::start()
        {
            LOG_INFO << "HttpServer[" << m_server.name()
                     << "] starts listenning on " << m_server.ipPort();
            m_server.start();
        }

        void HttpServer::onConnection(const net::TcpConnectionPtr &conn)
        {
            if (conn->connected())
            {
                conn->setContext(HttpContext());
            }
        }

        void HttpServer::onMessage(const net::TcpConnectionPtr &conn,
                                   net::Buffer *buf,
                                   Timestamp receiveTime)
        {
            HttpContext *context = std::any_cast<HttpContext>(conn->getMutableContext());

            if (!context->parseRequest(buf, receiveTime))
            {
                conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
                conn->shutdown();
            }

            if (context->gotAll())
            {
                onRequest(conn, context->request());
                context->reset();
            }
        }

        void HttpServer::onRequest(const net::TcpConnectionPtr &conn, const HttpRequest &req)
        {
            const std::string &connection = req.getHeader("Connection");
            bool close = connection == "close" ||
                         (req.getVersion() == HttpRequest::Http10 && connection != "Keep-Alive");
            HttpResponse response(close);
            m_httpCallback(req, &response);
            net::Buffer buf;
            response.appendToBuffer(&buf);
            conn->send(&buf);
            if (response.closeConnection())
            {
                conn->shutdown();
            }
        }
    }

}
