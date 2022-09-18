#include "HeadMethod.hpp"

#include <youth/net/http/HttpRequest.h>
#include <youth/net/http/HttpResponse.h>
#include <youth/utils/Logging.h>

#include <cassert>

using namespace youth::utils;

const std::string &HeadMethod::path(int index)
{
    assert(index >= 0 && index < m_paths.size());
    return m_paths[index];
}

void HeadMethod::call(int index, const HttpRequest &req, HttpResponse *resp)
{
    LOG_INFO << "Headers " << req.methodString() << " " << req.path();

    const std::map<std::string, std::string> &headers = req.headers();
    for (const auto &header : headers) {
        LOG_INFO << header.first << ": " << header.second;
    }

    assert(index >= 0 && index < m_paths.size());
    switch (index) {
    case 0:
        resp->setStatusCode(HttpResponse::k200Ok);
        resp->setStatusMessage("OK");
        resp->setContentType("text/plain");
        resp->addHeader("Server", "youth");
        resp->addHeader("Method", "This is Http HEAD Method");
        break;
    default: HttpMethodFactory::defaultCall(req, resp); break;
    }
    resp->setCloseConnection(true);
}
