#include "HttpMethod.hpp"
#include "HttpContext.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

#include <youth/core/Dir.h>
#include <youth/core/File.hpp>
#include <youth/net/TcpConnection.h>
#include <youth/utils/Logging.h>

#include <memory>

using namespace youth::utils;

namespace youth {

namespace http {

HttpMethodFactory::~HttpMethodFactory() {}

void HttpMethodFactory::readyRead(int index, const net::TcpConnectionPtr &, const HttpRequest &)
{
    return;
}

void HttpMethodFactory::call(int index, const HttpRequest &req, HttpResponse *resp)
{
    if (req.methodString() == m_method && req.path() == m_path) {
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
        resp->setContentType("text/html");
        resp->addHeader("Server", "youth");
        std::string now = DateTime::currentDateTime().toStandardFormat();
        resp->setBody("<html><head><title>This is title</title></head>"
                      "<body><h1>Hello</h1>Now is "
                      + now + "</body></html>");
    } else {
        defaultCall(req, resp);
    }
    resp->setCloseConnection(true);
}

void HttpMethodFactory::defaultCall(const HttpRequest &req, HttpResponse *resp)
{
    LOG_INFO << "Headers " << req.methodString() << " " << req.path();

    const std::map<std::string, std::string> &headers = req.headers();
    for (const auto &header : headers) {
        LOG_INFO << header.first << ": " << header.second;
    }

    resp->setStatusCode(HttpResponse::k404NotFound);
    resp->setStatusMessage("Not Found");
    resp->setCloseConnection(true);
}

struct HttpMethodBuilder::MethodDetail
{
    ~MethodDetail() {}

    std::weak_ptr<HttpMethodFactory> methodImpl;
    int index;
};

HttpMethodBuilder::HttpMethodBuilder() {}

HttpMethodBuilder::~HttpMethodBuilder() {}

void HttpMethodBuilder::registerMethod(std::shared_ptr<HttpMethodFactory> impl)
{
    for (int i = 0; i < impl->methodSize(); i++) {
        auto &list = m_methodMap[impl->method(i)];
        for (auto iter = list.begin(); iter != list.end();) {
            auto methodDetail = (*iter).get();
            auto share = methodDetail->methodImpl.lock();
            if (share) {
                if (share->path(methodDetail->index) == impl->method(i)) {
                    LOG_WARN << "Duplicate registration method.\nExisted method is: "
                             << share->method(methodDetail->index) << " "
                             << share->path(methodDetail->index)
                             << "\nThis method is: " << impl->method(i) << " " << impl->path(i);
                    return;
                }
                iter++;
            } else {
                iter = list.erase(iter);
            }
        }
        auto method = new MethodDetail{impl, i};
        list.emplace_back(method);
    }
}

void HttpMethodBuilder::onReadyRead(const net::TcpConnectionPtr &conn, HttpContext *context)
{
    const auto &req = context->request();
    auto &list = m_methodMap[req.methodString()];
    for (auto iter = list.begin(); iter != list.end();) {
        auto methodDetail = (*iter).get();
        auto share = methodDetail->methodImpl.lock();
        if (share) {
            if (share->path(methodDetail->index) == req.path()) {
                share->readyRead(methodDetail->index, conn, req);
                return;
            }
            iter++;
        } else {
            iter = list.erase(iter);
        }
    }

    putReadyRead(conn, context);
}

void HttpMethodBuilder::onRequest(const net::TcpConnectionPtr &conn,
                                  HttpContext *context,
                                  HttpResponse *resp)
{
    const auto &req = context->request();
    LOG_DEBUG << "Headers " << req.methodString() << " " << req.path();
    const std::map<std::string, std::string> &headers = req.headers();
    for (const auto &header : headers) {
        LOG_DEBUG << header.first << ": " << header.second;
    }

    auto &list = m_methodMap[req.methodString()];
    for (auto iter = list.begin(); iter != list.end();) {
        auto methodDetail = (*iter).get();
        auto share = methodDetail->methodImpl.lock();
        if (share) {
            if (share->path(methodDetail->index) == req.path()) {
                share->call(methodDetail->index, req, resp);
                return;
            }
            iter++;
        } else {
            iter = list.erase(iter);
        }
    }

    if (putFinish(context, resp)) {
        return;
    }

    if (getFile(conn, context, resp)) {
        return;
    }

    if (deletePath(context, resp)) {
        return;
    }

    HttpMethodFactory::defaultCall(req, resp);
}

void HttpMethodBuilder::putReadyRead(const net::TcpConnectionPtr &conn, HttpContext *context)
{
    const auto &req = context->request();
    if (req.method() != HttpRequest::Method::Put) {
        return;
    }
    auto any = context->getContext();
    LOG_INFO << "has_value: " << any.has_value();
    std::shared_ptr<File> filePtr;
    if (any.has_value()) {
        filePtr = std::any_cast<const std::shared_ptr<File> &>(any);
    } else {
        filePtr.reset(new File("." + req.path()));
        if (!filePtr->open(File::OpenMode::WriteOnly)) {
            HttpResponse response(true);
            response.setCloseConnection(true);
            response.setStatusCode(HttpResponse::k400BadRequest);
            net::Buffer buffer;
            response.appendToBuffer(&buffer);
            conn->shutdown();
            return;
        }
        context->setContext(filePtr);
    }

    auto buf = req.readAll();
    filePtr->write(buf.peek(), buf.readableBytes());
}

bool HttpMethodBuilder::putFinish(HttpContext *context, HttpResponse *resp)
{
    const auto &req = context->request();
    if (req.method() != HttpRequest::Method::Put) {
        return false;
    }
    auto filePtr = std::any_cast<const std::shared_ptr<File> &>(context->getContext());
    if (filePtr) {
        auto buf = req.readAll();
        filePtr->write(buf.peek(), buf.readableBytes());
        filePtr->flush();
        filePtr->close();
    }

    resp->setStatusCode(HttpResponse::k200Ok);
    resp->setStatusMessage("OK");
    resp->setContentType("text/plain");
    resp->addHeader("Server", "youth");
    resp->setCloseConnection(true);
    return true;
}

bool HttpMethodBuilder::getFile(const net::TcpConnectionPtr &conn,
                                HttpContext *context,
                                HttpResponse *resp)
{
    const auto &req = context->request();
    if (req.method() != HttpRequest::Method::Get) {
        return false;
    }

    std::shared_ptr<File> filePtr(new File("." + req.path()));
    if (!filePtr->open(File::OpenMode::ReadOnly)) {
        resp->setStatusCode(HttpResponse::k400BadRequest);
        resp->addHeader("Server", "youth");
        resp->setCloseConnection(true);
        return true;
    }

    context->setContext(filePtr);
    conn->setWriteCompleteCallback(
        std::bind(&HttpMethodBuilder::writeCompleteCallback, this, std::placeholders::_1));

    resp->setContentLength(filePtr->size());
    resp->setStatusCode(HttpResponse::k200Ok);
    resp->setStatusMessage("OK");
    resp->setContentType("text/plain");
    resp->addHeader("Server", "youth");
    return true;
}

void HttpMethodBuilder::writeCompleteCallback(const net::TcpConnectionPtr &conn)
{
    auto context = std::any_cast<HttpContext>(conn->getMutableContext());
    auto filePtr = std::any_cast<const std::shared_ptr<File> &>(context->getContext());

    auto buffer = filePtr->readMaxSize(m_bufSize);
    conn->send(buffer);
    if (filePtr->atEnd()) {
        conn->shutdown();
        LOG_INFO << "File Get - done";
    }
}

bool HttpMethodBuilder::deletePath(HttpContext *context, HttpResponse *resp)
{
    const auto &req = context->request();
    if (req.method() != HttpRequest::Method::Delete) {
        return false;
    }

    if (Dir::remove("." + req.path())) {
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
    } else {
        resp->setStatusCode(HttpResponse::k400BadRequest);
    }
    resp->setContentType("text/plain");
    resp->addHeader("Server", "youth");
    resp->setCloseConnection(true);
    return true;
}

} // namespace http

} // namespace youth
