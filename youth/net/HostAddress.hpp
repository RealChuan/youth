#pragma once

#include <youth/core/Object.h>

#include <netinet/in.h>
#include <string>
#include <vector>

namespace youth {

using namespace core;

namespace net {

class HostAddress : copyable
{
public:
    explicit HostAddress(uint16_t port = 0, bool loopbackOnly = false, bool ipv6 = false);
    explicit HostAddress(const char *ip, uint16_t port = 0, bool ipv6 = false);
    explicit HostAddress(const struct sockaddr_in &addr)
        : m_serveraddr(addr)
    {}
    explicit HostAddress(const struct sockaddr_in6 &addr)
        : m_serveraddr6(addr)
    {}

    bool operator==(const HostAddress &other) const;
    bool operator!=(const HostAddress &other) const { return !operator==(other); }

    const struct sockaddr *sockAddr() const
    {
        return reinterpret_cast<const sockaddr *>(&m_serveraddr6);
    }
    sa_family_t family() const { return m_serveraddr.sin_family; }
    std::string ip() const;
    uint16_t port() const;
    std::string ipAndPort() const;

    void setSockAddrInet6(const struct sockaddr_in6 &addr6) { m_serveraddr6 = addr6; }

    static std::vector<HostAddress> resolve(const std::string &hostname);

private:
    union {
        struct sockaddr_in m_serveraddr;
        struct sockaddr_in6 m_serveraddr6;
    };
};

} // namespace net

} // namespace youth
