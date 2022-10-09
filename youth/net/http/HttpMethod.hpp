#pragma once

#include "youth/net/TcpConnection.h"
#include <youth/core/Object.h>
#include <youth/net/Callbacks.h>

#include <functional>
#include <list>
#include <map>
#include <memory>
#include <string>

namespace youth {

namespace core {
class File;
}

namespace http {

class HttpRequest;
class HttpResponse;
class HttpContext;

class HttpMethodFactory : core::noncopyable
{
public:
    HttpMethodFactory()
        : m_method("GET")
        , m_path("/")
    {}

    virtual ~HttpMethodFactory();

    virtual int methodSize() const { return 1; }

    virtual const std::string &method(int index) { return m_method; }
    virtual const std::string &path(int index) { return m_path; }

    virtual void readyRead(int index, const net::TcpConnectionPtr &, const HttpRequest &);
    virtual void call(int index, const HttpRequest &, HttpResponse *);

    static void defaultCall(const HttpRequest &, HttpResponse *);

private:
    std::string m_method;
    std::string m_path;
};

class HttpMethodBuilder final : core::noncopyable
{
public:
    HttpMethodBuilder();
    ~HttpMethodBuilder();

    bool isEmpty() { return m_methodMap.empty(); };

    void registerMethod(std::shared_ptr<HttpMethodFactory> impl);
    void onReadyRead(const net::TcpConnectionPtr &, HttpContext *context);
    void onRequest(const net::TcpConnectionPtr &, HttpContext *context, HttpResponse *);

private:
    void putReadyRead(const net::TcpConnectionPtr &conn, HttpContext *context);
    bool putFinish(HttpContext *context, HttpResponse *);

    struct MethodDetail;
    using MethodDetailList = std::list<std::unique_ptr<MethodDetail>>;
    std::map<std::string, MethodDetailList> m_methodMap;
};

} // namespace http

} // namespace youth
