#ifndef TCPADDRESSINFO_H
#define TCPADDRESSINFO_H

#include <youth/core/Object.h>

#include <netinet/in.h>
#include <string>

namespace youth
{

using namespace core;

namespace net
{

class TcpAddressInfo : copyable
{
public:
    explicit TcpAddressInfo(uint16_t port = 0,
                            bool loopbackOnly = false,
                            bool ipv6 = false);

    TcpAddressInfo(const char *ip, uint16_t port = 0, bool ipv6 = false);

    const struct sockaddr* getSockAddr() const;
    sa_family_t family() const;
    std::string ip() const;
    uint16_t port() const;
    std::string ipAndPort() const;

    void setSockAddrInet6(const struct sockaddr_in6& addr6);

    // resolve hostname to IP address, not changing port or sin_family
    // return true on success.
    // thread safe
    static bool resolve(std::string hostname, TcpAddressInfo* result);

private:
    union
    {
        struct sockaddr_in m_serveraddr;
        struct sockaddr_in6 m_serveraddr6;
    };
};

}

}

#endif // TCPADDRESSINFO_H
