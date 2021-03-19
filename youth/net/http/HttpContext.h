#ifndef HTTPCONTEXT_H
#define HTTPCONTEXT_H

#include "HttpRequest.h"

namespace youth
{

    namespace net
    {
        class Buffer;
    }

    namespace http
    {

        class HttpContext : public core::copyable
        {
        public:
            enum HttpRequestParseState
            {
                ExpectRequestLine,
                ExpectHeaders,
                ExpectBody,
                GotAll,
            };

            HttpContext()
                : m_state(ExpectRequestLine)
            {
            }

            bool parseRequest(net::Buffer *buf, Timestamp receiveTime);

            bool gotAll() const
            {
                return m_state == GotAll;
            }

            void reset()
            {
                m_state = ExpectRequestLine;
                HttpRequest dummy;
                m_request.swap(dummy);
            }

            const HttpRequest &request() const
            {
                return m_request;
            }

            HttpRequest &request()
            {
                return m_request;
            }

        private:
            bool processRequestLine(const char *begin, const char *end);

            HttpRequestParseState m_state;
            HttpRequest m_request;
        };

    }

}

#endif // HTTPCONTEXT_H
