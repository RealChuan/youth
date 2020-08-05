#include "Socket.h"
#include "SocketFunc.h"

#include <youth/utils/LogOut.h>

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <assert.h>
#include <netdb.h>
#include <sys/socket.h>

using namespace youth;

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

/*
 *
 */

Socket::Socket(int sockfd)
    : m_sockfd(sockfd)
{

}

Socket::~Socket()
{
    SocketFunc::close(m_sockfd);
}

int Socket::fd() const
{
    return m_sockfd;
}

bool Socket::getTcpInfo(tcp_info *tcpi) const
{
    socklen_t len = sizeof(*tcpi);
    memset(tcpi, 0, len);
    return ::getsockopt(m_sockfd, SOL_TCP, TCP_INFO, tcpi, &len) == 0;
}

bool Socket::getTcpInfoString(char *buf, int len) const
{
    struct tcp_info tcpi;
    bool ok = getTcpInfo(&tcpi);
    if (ok)
    {
        snprintf(buf, len, "unrecovered=%u "
                           "rto=%u ato=%u snd_mss=%u rcv_mss=%u "
                           "lost=%u retrans=%u rtt=%u rttvar=%u "
                           "sshthresh=%u cwnd=%u total_retrans=%u",
                 tcpi.tcpi_retransmits,  // Number of unrecovered [RTO] timeouts
                 tcpi.tcpi_rto,          // Retransmit timeout in usec
                 tcpi.tcpi_ato,          // Predicted tick of soft clock in usec
                 tcpi.tcpi_snd_mss,
                 tcpi.tcpi_rcv_mss,
                 tcpi.tcpi_lost,         // Lost packets
                 tcpi.tcpi_retrans,      // Retransmitted packets out
                 tcpi.tcpi_rtt,          // Smoothed round trip time in usec
                 tcpi.tcpi_rttvar,       // Medium deviation
                 tcpi.tcpi_snd_ssthresh,
                 tcpi.tcpi_snd_cwnd,
                 tcpi.tcpi_total_retrans);  // Total retransmits for entire connection
    }
    return ok;
}

void Socket::bindAddress(const TcpAddressInfo &addrInfo)
{
    SocketFunc::bind(m_sockfd, addrInfo.getSockAddr());
}

void Socket::listen(const TcpAddressInfo &addrInfo)
{
    SocketFunc::listen(m_sockfd, addrInfo.getSockAddr());
}

int Socket::accept(TcpAddressInfo *peeraddr)
{
    struct sockaddr_in6 addr;
    memset(&addr, 0, sizeof addr);
    int connfd = SocketFunc::accept(m_sockfd, &addr);
    if(connfd > 0)
    {
        peeraddr->setSockAddrInet6(addr);
    }
    return connfd;
}

void Socket::shutdownWrite()
{
    SocketFunc::shutdownWrite(m_sockfd);
}

void Socket::setTcpNoDelay(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(m_sockfd, IPPROTO_TCP, TCP_NODELAY,
                 &optval, static_cast<socklen_t>(sizeof optval));
}

void Socket::setReuseAddr(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR,
                 &optval, static_cast<socklen_t>(sizeof optval));
}

void Socket::setReusePort(bool on)
{
#ifdef SO_REUSEPORT
    int optval = on ? 1 : 0;
    int ret = ::setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEPORT,
                           &optval, static_cast<socklen_t>(sizeof optval));
    if (ret < 0 && on)
    {
        LOG_ERROR << "SO_REUSEPORT failed.";
    }
#else
    if (on)
    {
        LOG_ERROR << "SO_REUSEPORT is not supported.";
    }
#endif
}

void Socket::setKeepAlive(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(m_sockfd, SOL_SOCKET, SO_KEEPALIVE,
                 &optval, static_cast<socklen_t>(sizeof optval));
}
