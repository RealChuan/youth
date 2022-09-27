#include "PostMethod.hpp"

#include <string_view>
#include <youth/net/http/HttpRequest.h>
#include <youth/net/http/HttpResponse.h>
#include <youth/utils/Logging.h>

#include <cassert>

using namespace youth::utils;

const std::string &PostMethod::path(int index)
{
    assert(index >= 0 && index < m_paths.size());
    return m_paths[index];
}

void PostMethod::call(int index, const HttpRequest &req, HttpResponse *resp)
{
    LOG_INFO << "Headers " << req.methodString() << " " << req.path();

    const std::map<std::string, std::string> &headers = req.headers();
    for (const auto &header : headers) {
        LOG_INFO << header.first << ": " << header.second;
    }

    auto body = req.readAll();
    LOG_INFO << "Body: " << std::string(body.peek(), body.readableBytes());

    assert(index >= 0 && index < m_paths.size());
    switch (index) {
    case 0:
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
        resp->setContentType("text/plain");
        resp->addHeader("Server", "youth");
        resp->setBody("This is Http POST Method");
        break;
    default: HttpMethodFactory::defaultCall(req, resp); break;
    }
    resp->setCloseConnection(true);
}
