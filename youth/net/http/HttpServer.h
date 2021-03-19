#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <youth/net/TcpServer.h>
#include <youth/net/Callbacks.h>

namespace youth
{
    namespace net
    {
        class EventLoop;
        class TcpAddressInfo;
        class Buffer;
    }

    namespace http
    {
        class HttpRequest;
        class HttpResponse;
        class HttpServer : core::noncopyable
        {
        public:
            typedef std::function<void(const HttpRequest &, HttpResponse *)> HttpCallback;

            HttpServer(net::EventLoop *loop,
                       const net::TcpAddressInfo &listenAddr,
                       const std::string &name,
                       net::TcpServer::Option option = net::TcpServer::kNoReusePort);

            net::EventLoop *getLoop() const { return m_server.getLoop(); }

            void setHttpCallback(const HttpCallback &cb)
            {
                m_httpCallback = cb;
            }

            void setThreadNum(int numThreads)
            {
                m_server.setThreadNum(numThreads);
            }

            void start();

        private:
            void onConnection(const net::TcpConnectionPtr &conn);
            void onMessage(const net::TcpConnectionPtr &conn,
                           net::Buffer *buf,
                           Timestamp receiveTime);
            void onRequest(const net::TcpConnectionPtr &, const HttpRequest &);

            net::TcpServer m_server;
            HttpCallback m_httpCallback;
        };
    }

} // namespace youth

#endif // HTTPSERVER_H