#include "HttpMethod.hpp"
#include "HttpRequest.h"
#include "HttpResponse.h"

#include <youth/utils/Logging.h>

using namespace youth::utils;

namespace youth {

namespace http {

HttpMethodFactory::~HttpMethodFactory() {}

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

void HttpMethodBuilder::onRequest(const HttpRequest &req, HttpResponse *resp)
{
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
    HttpMethodFactory::defaultCall(req, resp);
}

} // namespace http

} // namespace youth
