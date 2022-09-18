#include "HttpRequest.h"

namespace youth {

namespace http {

void HttpRequest::swap(HttpRequest &other)
{
    std::swap(m_method, other.m_method);
    std::swap(m_version, other.m_version);
    m_path.swap(other.m_path);
    m_query.swap(other.m_query);
    m_receiveTime.swap(other.m_receiveTime);
    m_headers.swap(other.m_headers);
    m_body.swap(other.m_body);
}

bool HttpRequest::setMethod(const char *start, const char *end)
{
    assert(m_method == Invalid);
    std::string m(start, end);
    if (m == "GET") {
        m_method = Get;
    } else if (m == "POST") {
        m_method = Post;
    } else if (m == "HEAD") {
        m_method = Head;
    } else if (m == "PUT") {
        m_method = Put;
    } else if (m == "DELETE") {
        m_method = Delete;
    } else {
        m_method = Invalid;
    }

    return m_method != Invalid;
}

const char *HttpRequest::methodString() const
{
    const char *result = "UNKNOWN";
    switch (m_method) {
    case Get: result = "GET"; break;
    case Post: result = "POST"; break;
    case Head: result = "HEAD"; break;
    case Put: result = "PUT"; break;
    case Delete: result = "DELETE"; break;
    default: break;
    }
    return result;
}

void HttpRequest::addHeader(const char *start, const char *colon, const char *end)
{
    std::string field(start, colon);
    ++colon;
    while (colon < end && isspace(*colon)) {
        ++colon;
    }
    std::string value(colon, end);
    while (!value.empty() && isspace(value[value.size() - 1])) {
        value.resize(value.size() - 1);
    }
    m_headers[field] = value;
}

std::string HttpRequest::getHeader(const std::string &field) const
{
    std::string result;
    std::map<std::string, std::string>::const_iterator it = m_headers.find(field);
    if (it != m_headers.end()) {
        result = it->second;
    }
    return result;
}

} // namespace http

} // namespace youth
