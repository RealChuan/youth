#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <youth/core/DateTime.hpp>
#include <youth/core/Object.h>
#include <youth/net/Buffer.h>

#include <assert.h>
#include <map>

namespace youth {

using namespace core;

namespace http {

class HttpRequest : core::copyable
{
public:
    enum Method { Invalid, Get, Post, Head, Put, Delete };

    enum Version { Unknown, Http10, Http11 };

    HttpRequest()
        : m_method(Invalid)
        , m_version(Unknown)
    {}

    ~HttpRequest() {}

    void swap(HttpRequest &other);

    void setVersion(Version version) { m_version = version; }

    Version getVersion() const { return m_version; }

    bool setMethod(const char *start, const char *end);
    Method method() const { return m_method; }
    const char *methodString() const;

    void setPath(const char *start, const char *end) { m_path.assign(start, end); }

    const std::string &path() const { return m_path; }

    void setQuery(const char *start, const char *end) { m_query.assign(start, end); }

    void setReceiveTime(DateTime t) { m_receiveTime = t; }

    DateTime receiveTime() const { return m_receiveTime; }

    void addHeader(const char *start, const char *colon, const char *end);
    std::string getHeader(const std::string &field) const;

    const std::map<std::string, std::string> &headers() const { return m_headers; }

    size_t contentLength();
    void appendBody(const char *start, size_t len);
    bool gotAllBody();
    net::Buffer readAll() const
    {
        m_readSize += m_body.readableBytes();
        net::Buffer buf;
        buf.swap(m_body);
        return buf;
    }

private:
    void initContentLength();

    Method m_method;
    Version m_version;
    std::string m_path;
    std::string m_query;
    DateTime m_receiveTime;
    std::map<std::string, std::string> m_headers;
    mutable net::Buffer m_body;
    size_t m_content_length = 0;
    mutable size_t m_readSize = 0;
};

} // namespace http

} // namespace youth

#endif // HTTPREQUEST_H
