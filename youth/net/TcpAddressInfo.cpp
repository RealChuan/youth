#include "TcpAddressInfo.h"
#include "SocketFunc.h"

#include <youth/utils/Logging.h>

#include <assert.h>
#include <netdb.h>

namespace youth
{

using namespace utils;

namespace net
{

TcpAddressInfo::TcpAddressInfo(uint16_t port, bool loopbackOnly, bool ipv6)
{
    if(ipv6)
        SocketFunc::setServerAddress(port, &m_serveraddr6, loopbackOnly);
    else
        SocketFunc::setServerAddress(port, &m_serveraddr, loopbackOnly);
}

TcpAddressInfo::TcpAddressInfo(const char *ip, uint16_t port, bool ipv6)
{
    if(ipv6)
        SocketFunc::setServerAddress(ip, port, &m_serveraddr6);
    else
        SocketFunc::setServerAddress(ip, port, &m_serveraddr);
}

TcpAddressInfo::TcpAddressInfo(const sockaddr_in &addr)
    : m_serveraddr(addr)
{
}

TcpAddressInfo::TcpAddressInfo(const sockaddr_in6 &addr)
    : m_serveraddr6(addr)
{
}

const sockaddr *TcpAddressInfo::getSockAddr() const
{
    return reinterpret_cast<const sockaddr*>(&m_serveraddr);
}

sa_family_t TcpAddressInfo::family() const
{
    return m_serveraddr.sin_family;
}

std::string TcpAddressInfo::ip() const
{
    return SocketFunc::getIp(reinterpret_cast<const struct sockaddr*>(&m_serveraddr6));
}

uint16_t TcpAddressInfo::port() const
{
    return SocketFunc::getPort(reinterpret_cast<const struct sockaddr*>(&m_serveraddr6));
}

std::string TcpAddressInfo::ipAndPort() const
{
    return SocketFunc::getIpAndPort(reinterpret_cast<const struct sockaddr*>(&m_serveraddr6));
}

void TcpAddressInfo::setSockAddrInet6(const sockaddr_in6 &addr6)
{
    m_serveraddr6 = addr6;
}

static __thread char t_resolveBuf[64 * 1024];

bool TcpAddressInfo::resolve(std::string hostname, TcpAddressInfo *out)
{
    assert(out != NULL);
    struct hostent hent;
    struct hostent* he = NULL;
    int herrno = 0;
    memset(&hent, 0, sizeof(hent));

    int ret = gethostbyname_r(hostname.c_str(), &hent, t_resolveBuf,
                              sizeof t_resolveBuf, &he, &herrno);
    if (ret == 0 && he != NULL)
    {
        assert(he->h_addrtype == AF_INET && he->h_length == sizeof(uint32_t));
        out->m_serveraddr.sin_addr = *reinterpret_cast<struct in_addr*>(he->h_addr);
        return true;
    }
    else
    {
        if (ret)
        {
            LOG_ERROR << "TcpAddressInfo::resolve";
        }
        return false;
    }
}

}

}