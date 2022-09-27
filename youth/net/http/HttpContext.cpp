#include "HttpContext.h"

#include <youth/net/Buffer.h>
#include <youth/utils/Logging.h>

#include <algorithm>
#include <string>

namespace youth {

using namespace utils;

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
                    if (m_request.contentLength() == 0) {
                        m_state = GotAll;
                    } else {
                        // empty line, end of header
                        // FIXME:
                        m_state = ExpectBody;
                    }
                    hasMore = false;
                }
                buf->retrieveUntil(crlf + 2);
            } else {
                hasMore = false;
            }
        } else if (m_state == ExpectBody) {
            if (m_request.contentLength() == 0) {
                m_state = GotAll;
            } else {
                m_request.appendBody(buf->peek(), buf->readableBytes());
                buf->retrieveAll();
            }
            if (m_state != GotAll && m_request.gotAllBody()) {
                m_state = GotAll;
            }
            hasMore = false;
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
