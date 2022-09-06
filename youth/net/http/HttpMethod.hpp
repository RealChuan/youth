#pragma once

#include <youth/core/Object.h>

#include <functional>
#include <list>
#include <map>
#include <memory>
#include <string>

namespace youth {

namespace http {

class HttpRequest;
class HttpResponse;

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

    virtual void call(int index, const HttpRequest &, HttpResponse *);

    static void defaultCall(const HttpRequest &, HttpResponse *);

private:
    std::string m_method;
    std::string m_path;
};

class HttpMethodBuilder final : core::noncopyable
{
public:
    HttpMethodBuilder() {}
    ~HttpMethodBuilder();

    bool isEmpty() { return m_methodMap.empty(); };

    void registerMethod(std::shared_ptr<HttpMethodFactory> impl);
    void onRequest(const HttpRequest &, HttpResponse *);

private:
    struct MethodDetail
    {
        ~MethodDetail() {}

        std::weak_ptr<HttpMethodFactory> methodImpl;
        int index;
    };
    using MethodDetailList = std::list<std::unique_ptr<MethodDetail>>;
    std::map<std::string, MethodDetailList> m_methodMap;
};

} // namespace http

} // namespace youth
