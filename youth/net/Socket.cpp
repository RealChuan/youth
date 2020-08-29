#include "Socket.h"
#include "SocketFunc.h"
#include "TcpAddressInfo.h"

#include <youth/utils/Logging.h>

#include <netinet/tcp.h>

namespace youth
{

using namespace utils;

namespace net
{

Socket::Socket(int sockfd)
    : m_sockfd(sockfd)
{
}

Socket::~Socket()
{
    SocketFunc::close(m_sockfd);
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

void Socket::listen()
{
    SocketFunc::listen(m_sockfd);
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

}

}
