#ifndef HTTPCONTEXT_H
#define HTTPCONTEXT_H

#include "HttpRequest.h"

#include <any>

namespace youth {

namespace net {
class Buffer;
}

namespace http {

class HttpContext : public core::copyable
{
public:
    enum HttpRequestParseState {
        ExpectRequestLine,
        ExpectHeaders,
        ExpectBody,
        GotAll,
    };

    HttpContext();
    ~HttpContext();

    bool parseRequest(net::Buffer *buf, DateTime receiveTime);

    bool gotAll() const { return m_state == GotAll; }

    bool gotBody() const { return m_state == ExpectBody; }

    void reset()
    {
        m_state = ExpectRequestLine;
        HttpRequest dummy;
        m_request.swap(dummy);
    }

    const HttpRequest &request() const { return m_request; }

    HttpRequest &request() { return m_request; }

    void setContext(const std::any &context) { m_context = context; }
    const std::any &getContext() const { return m_context; }
    std::any *getMutableContext() { return &m_context; }

private:
    bool processRequestLine(const char *begin, const char *end);

    HttpRequestParseState m_state;
    HttpRequest m_request;
    std::any m_context;
};

} // namespace http

} // namespace youth

#endif // HTTPCONTEXT_H
