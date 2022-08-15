#include "HttpContext.h"

#include <youth/net/Buffer.h>

#include <algorithm>

namespace youth {

namespace http {
bool HttpContext::parseRequest(net::Buffer *buf, DateTime receiveTime)
{
    bool ok = true;
    bool hasMore = true;
    while (hasMore) {
        if (m_state == ExpectRequestLine) {
            const char *crlf = buf->findCRLF();
            if (crlf) {
                ok = processRequestLine(buf->peek(), crlf);
                if (ok) {
                    m_request.setReceiveTime(receiveTime);
                    buf->retrieveUntil(crlf + 2);
                    m_state = ExpectHeaders;
                } else {
                    hasMore = false;
                }
            } else {
                hasMore = false;
            }
        } else if (m_state == ExpectHeaders) {
            const char *crlf = buf->findCRLF();
            if (crlf) {
                const char *colon = std::find(buf->peek(), crlf, ':');
                if (colon != crlf) {
                    m_request.addHeader(buf->peek(), colon, crlf);
                } else {
                    // empty line, end of header
                    // FIXME:
                    m_state = GotAll;
                    hasMore = false;
                }
                buf->retrieveUntil(crlf + 2);
            } else {
                hasMore = false;
            }
        } else if (m_state == ExpectBody) {
            // FIXME:
        }
    }
    return ok;
}

bool HttpContext::processRequestLine(const char *begin, const char *end)
{
    bool succeed = false;
    const char *start = begin;
    const char *space = std::find(start, end, ' ');
    if (space != end && m_request.setMethod(start, space)) {
        start = space + 1;
        space = std::find(start, end, ' ');
        if (space != end) {
            const char *question = std::find(start, space, '?');
            if (question != space) {
                m_request.setPath(start, question);
                m_request.setQuery(question, space);
            } else {
                m_request.setPath(start, space);
            }
            start = space + 1;
            succeed = end - start == 8 && std::equal(start, end - 1, "HTTP/1.");
            if (succeed) {
                if (*(end - 1) == '1') {
                    m_request.setVersion(HttpRequest::Http11);
                } else if (*(end - 1) == '0') {
                    m_request.setVersion(HttpRequest::Http10);
                } else {
                    succeed = false;
                }
            }
        }
    }
    return succeed;
}
} // namespace http

} // namespace youth
