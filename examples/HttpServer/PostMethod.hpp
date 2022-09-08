#pragma once

#include <youth/net/http/HttpMethod.hpp>

using namespace youth::http;

class PostMethod : public HttpMethodFactory
{
public:
    PostMethod()
        : m_method("POST")
        , m_paths({"/POST"})
    {}

    virtual int methodSize() const override { return m_paths.size(); }

    virtual const std::string &method(int index) override { return m_method; }
    virtual const std::string &path(int index) override;
    virtual void call(int index, const HttpRequest &req, HttpResponse *resp) override;

private:
    std::string m_method;
    std::vector<std::string> m_paths;
};
