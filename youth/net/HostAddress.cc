#include "HostAddress.hpp"
#include "SocketFunc.h"

#include <youth/utils/Logging.h>

#include <assert.h>
#include <netdb.h>

namespace youth {

using namespace utils;

namespace net {

HostAddress::HostAddress(uint16_t port, bool loopbackOnly, bool ipv6)
{
    if (ipv6) {
        SocketFunc::setServerAddress(port, &m_serveraddr6, loopbackOnly);
    } else {
        SocketFunc::setServerAddress(port, &m_serveraddr, loopbackOnly);
    }
}

HostAddress::HostAddress(const char *ip, uint16_t port, bool ipv6)
{
    if (ipv6) {
        SocketFunc::setServerAddress(ip, port, &m_serveraddr6);
    } else {
        SocketFunc::setServerAddress(ip, port, &m_serveraddr);
    }
}

bool HostAddress::operator==(const HostAddress &other) const
{
    if (m_serveraddr.sin_family != other.m_serveraddr.sin_family) {
        return false;
    }

    return ipAndPort() == other.ipAndPort();
}

std::string HostAddress::ip() const
{
    return SocketFunc::getIp(sockAddr());
}

uint16_t HostAddress::port() const
{
    return SocketFunc::getPort(sockAddr());
}

std::string HostAddress::ipAndPort() const
{
    return SocketFunc::getIpAndPort(sockAddr());
}

std::vector<HostAddress> HostAddress::resolve(std::string_view hostname)
{
    std::vector<HostAddress> infos;
    addrinfo *res = nullptr;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    int result = getaddrinfo(hostname.data(), nullptr, &hints, &res);
    if (result == 0) {
        addrinfo *node = res;
        while (node) {
            LOG_DEBUG << "getaddrinfo node: flags:" << node->ai_flags
                      << "family:" << node->ai_family << "ai_socktype:" << node->ai_socktype
                      << "ai_protocol:" << node->ai_protocol << "ai_addrlen:" << node->ai_addrlen;
            switch (node->ai_family) {
            case AF_INET: {
                HostAddress info(*reinterpret_cast<sockaddr_in *>(node->ai_addr));
                if (infos.end() == std::find(infos.begin(), infos.end(), info)) {
                    infos.push_back(info);
                }
                break;
            }
            case AF_INET6: {
                HostAddress info(*reinterpret_cast<sockaddr_in6 *>(node->ai_addr));
                if (infos.end() == std::find(infos.begin(), infos.end(), info)) {
                    infos.push_back(info);
                }
                break;
            }
            default: LOG_ERROR << "HostAddress::resolve"; break;
            }
            node = node->ai_next;
        }
        freeaddrinfo(res);
    } else {
        switch (result) {
        case EAI_NONAME:
        case EAI_FAIL:
#ifdef EAI_NODATA // EAI_NODATA is deprecated in RFC 3493
        case EAI_NODATA:
#endif
            LOG_ERROR << "Host not found: " << gai_strerror(result);
            break;
        default: LOG_ERROR << "Host not found: " << gai_strerror(result); break;
        }
    }
    return infos;
}

} // namespace net

} // namespace youth
