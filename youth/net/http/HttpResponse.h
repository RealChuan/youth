#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <youth/core/Object.h>

#include <map>

namespace youth
{

    namespace net
    {
        class Buffer;
    }

    namespace http
    {
        class HttpResponse : core::noncopyable
        {
        public:
            enum HttpStatusCode
            {
                kUnknown,
                k200Ok = 200,
                k301MovedPermanently = 301,
                k400BadRequest = 400,
                k404NotFound = 404,
            };

            HttpResponse(bool close)
                : m_closeConnection(close)
            {
            }

            void setStatusCode(HttpStatusCode code)
            {
                m_statusCode = code;
            }

            void setStatusMessage(const std::string &message)
            {
                m_statusMessage = message;
            }

            void setCloseConnection(bool on)
            {
                m_closeConnection = on;
            }

            bool closeConnection() const
            {
                return m_closeConnection;
            }

            void setContentType(const std::string &contentType)
            {
                addHeader("Content-Type", contentType);
            }

            // FIXME: replace string with StringPiece
            void addHeader(const std::string &key, const std::string &value)
            {
                m_headers[key] = value;
            }

            void setBody(const std::string &body)
            {
                m_body = body;
            }

            void appendToBuffer(net::Buffer *output) const;

        private:
            std::map<std::string, std::string> m_headers;
            HttpStatusCode m_statusCode = kUnknown;
            // FIXME: add http version
            std::string m_statusMessage;
            bool m_closeConnection;
            std::string m_body;
        };
    }

}

#endif // HTTPRESPONSE_H
