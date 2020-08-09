#include "SocketFunc.h"
#include "youth/utils/Logging.h"

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/uio.h>

namespace youth
{

using namespace utils;

namespace net
{

void SocketFunc::setServerAddress(uint16_t port, sockaddr_in *serverAddr,
                                  bool loopbackOnly)
{
    memset(serverAddr, 0, sizeof(struct sockaddr_in));
    serverAddr->sin_family = AF_INET;
    serverAddr->sin_port = hostToNetwork16(port);
    in_addr_t ip = loopbackOnly ? INADDR_LOOPBACK : INADDR_ANY;
    serverAddr->sin_addr.s_addr = htobe32(ip);
}

void SocketFunc::setServerAddress(uint16_t port, sockaddr_in6 *serverAddr6,
                                  bool loopbackOnly)
{
    memset(serverAddr6, 0, sizeof(struct sockaddr_in));
    serverAddr6->sin6_family = AF_INET6;
    serverAddr6->sin6_port = hostToNetwork16(port);
    in6_addr ip = loopbackOnly ? in6addr_loopback : in6addr_any;
    serverAddr6->sin6_addr = ip;
}

void SocketFunc::setServerAddress(const char *ip, uint16_t port,
                                  sockaddr_in *serverAddr)
{
    memset(serverAddr, 0, sizeof(struct sockaddr_in));
    serverAddr->sin_family = AF_INET;
    serverAddr->sin_port = hostToNetwork16(port);
    if (::inet_pton(AF_INET, ip, &serverAddr->sin_addr) <= 0)
    {
        LOG_ERROR << "SocketFunc::serServerAddr";
    }
}

void SocketFunc::setServerAddress(const char *ip, uint16_t port,
                                  sockaddr_in6 *serverAddr6)
{
    memset(serverAddr6, 0, sizeof(struct sockaddr_in));
    serverAddr6->sin6_family = AF_INET;
    serverAddr6->sin6_port = hostToNetwork16(port);
    if (::inet_pton(AF_INET6, ip, &serverAddr6->sin6_addr) <= 0)
    {
        LOG_ERROR << "SocketFunc::serServerAddr";
    }
}

int SocketFunc::createNonblockingOrDie(sa_family_t family)
{
#if VALGRIND
    int sockfd = ::socket(family, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
    {
        LOG_FATAL << "SocketFunc::createNonblockingOrDie";
    }

    setNonBlockAndCloseOnExec(sockfd);
#else
    int sockfd = ::socket(family, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if (sockfd < 0)
    {
        LOG_FATAL << "SocketFunc::createNonblockingOrDie";
    }
#endif
    return sockfd;
}

void SocketFunc::bind(const int fd, const sockaddr *addr)
{
    int ret = ::bind(fd, reinterpret_cast<const sockaddr *>(addr),
                     socklen_t(sizeof(struct sockaddr_in)));
    if (ret < 0)
    {
        LOG_FATAL << "bind error:" << errno;
        return;
    }
}

void SocketFunc::listen(const int serverfd, const sockaddr *serverAddr)
{
    int ret_ = ::listen(serverfd, SOMAXCONN);
    if (ret_ < 0)
    {
        LOG_FATAL << "listen error:" << errno;
        return;
    }

    LOG_INFO << "Server: " << SocketFunc::getIpAndPort(serverAddr) << "Listen...";
}

int SocketFunc::accept(const int serverfd, const sockaddr_in6 *clientAddr)
{
    LOG_INFO << "Wait Client Connect...";
    socklen_t addrlen = static_cast<socklen_t>(sizeof clientAddr);
#if VALGRIND || defined (NO_ACCEPT4)
    int connfd = ::accept(serverfd, reinterpret_cast<struct sockaddr*>(&clientAddr),
                          &addrlen);
    setNonBlockAndCloseOnExec(connfd);
#else
    int connfd = ::accept4(serverfd, reinterpret_cast<struct sockaddr*>(&clientAddr),
                           &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
#endif
    if (connfd < 0)
    {
        int savedErrno = errno;
        LOG_ERROR << "SocketFunc::accept";
        switch (savedErrno)
        {
        case EAGAIN:
        case ECONNABORTED:
        case EINTR:
        case EPROTO: // ???
        case EPERM:
        case EMFILE: // per-process lmit of open file desctiptor ???
            // expected errors
            errno = savedErrno;
            break;
        case EBADF:
        case EFAULT:
        case EINVAL:
        case ENFILE:
        case ENOBUFS:
        case ENOMEM:
        case ENOTSOCK:
        case EOPNOTSUPP:
            // unexpected errors
            LOG_FATAL << "unexpected error of ::accept " << savedErrno;
            break;
        default:
            LOG_FATAL << "unknown error of ::accept " << savedErrno;
            break;
        }
    }
    LOG_INFO << "Client Online: " << SocketFunc::getIpAndPort(reinterpret_cast<const sockaddr*>(clientAddr));
    return connfd;
}

int SocketFunc::connect(int sockfd, const sockaddr *addr)
{
    int ret = connect(sockfd, reinterpret_cast<struct sockaddr *>(&addr),
                      static_cast<socklen_t>(sizeof addr));
    if (ret < 0)
    {
        LOG_ERROR << "connect Server error";
        return ret;
    }

    LOG_INFO << "Successfully connected to the server: " << SocketFunc::getIpAndPort(addr);
    return ret;
}

void SocketFunc::shutdownWrite(int sockfd)
{
    if (::shutdown(sockfd, SHUT_WR) < 0)
    {
        LOG_ERROR << "SocketFunc::shutdownWrite";
    }
}

void SocketFunc::close(int sockfd)
{
    if (sockfd > 0)
    {
        if (::close(sockfd) < 0)
        {
            LOG_ERROR << "SocketFunc::close";
        }
    }
}

std::string SocketFunc::getIp(const sockaddr *addr)
{
    char buf[64];   //INET_ADDRSTRLEN INET6_ADDRSTRLEN
    if(addr->sa_family == AF_INET)
    {
        const struct sockaddr_in* addr4 =
                reinterpret_cast<const struct sockaddr_in*>(addr);
        ::inet_ntop(AF_INET, &addr4->sin_addr, buf,
                    static_cast<socklen_t>(static_cast<socklen_t>(sizeof buf)));
    }
    else if(addr->sa_family == AF_INET6)
    {
        const struct sockaddr_in6* addr6 =
                reinterpret_cast<const struct sockaddr_in6*>(addr);
        ::inet_ntop(AF_INET6, &addr6->sin6_addr, buf,
                    static_cast<socklen_t>(sizeof buf));

    }
    return buf;
}

uint16_t SocketFunc::getPort(const sockaddr *addr)
{
    const struct sockaddr_in* addr4 =
            reinterpret_cast<const struct sockaddr_in*>(addr);
    return be16toh(addr4->sin_port);
}

std::string SocketFunc::getIpAndPort(const sockaddr *addr)
{
    std::string ip = SocketFunc::getIp(addr);
    uint16_t port = SocketFunc::getPort(addr);
    char buf[12];
    snprintf(buf, sizeof buf, ":%u", port);
    return ip + buf;
}

ssize_t SocketFunc::read(int sockfd, void *buf, size_t count)
{
    return ::read(sockfd, buf, count);
}

ssize_t SocketFunc::readv(int sockfd, const struct iovec *iov, int iovcnt)
{
    return ::readv(sockfd, iov, iovcnt);
}

ssize_t SocketFunc::write(int sockfd, const void *buf, size_t count)
{
    return ::write(sockfd, buf, count);
}

int SocketFunc::getSocketError(int sockfd)
{
    int optval;
    socklen_t optlen = static_cast<socklen_t>(sizeof optval);

    if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0)
    {
        return errno;
    }
    return optval;
}

bool SocketFunc::isSelfConnect(int sockfd)
{
    struct sockaddr_in6 localaddr = getLocalAddr(sockfd);
    struct sockaddr_in6 peeraddr = getPeerAddr(sockfd);
    if (localaddr.sin6_family == AF_INET)
    {
        const struct sockaddr_in* laddr4 = reinterpret_cast<struct sockaddr_in*>(&localaddr);
        const struct sockaddr_in* raddr4 = reinterpret_cast<struct sockaddr_in*>(&peeraddr);
        return laddr4->sin_port == raddr4->sin_port
                && laddr4->sin_addr.s_addr == raddr4->sin_addr.s_addr;
    }
    else if (localaddr.sin6_family == AF_INET6)
    {
        return localaddr.sin6_port == peeraddr.sin6_port
                && memcmp(&localaddr.sin6_addr, &peeraddr.sin6_addr, sizeof localaddr.sin6_addr) == 0;
    }
    return false;
}

sockaddr_in6 SocketFunc::getLocalAddr(int sockfd)
{
    struct sockaddr_in6 localaddr;
    memset(&localaddr, 0, sizeof localaddr);
    socklen_t addrlen = static_cast<socklen_t>(sizeof localaddr);
    if (::getsockname(sockfd, reinterpret_cast<struct sockaddr*>(&localaddr), &addrlen) < 0)
    {
        LOG_ERROR << "SocketFunc::getLocalAddr";
    }
    return localaddr;
}

sockaddr_in6 SocketFunc::getPeerAddr(int sockfd)
{
    struct sockaddr_in6 peeraddr;
    memset(&peeraddr, 0, sizeof peeraddr);
    socklen_t addrlen = static_cast<socklen_t>(sizeof peeraddr);
    if (::getpeername(sockfd, reinterpret_cast<struct sockaddr*>(&peeraddr), &addrlen) < 0)
    {
        LOG_ERROR << "SocketFunc::getPeerAddr";
    }
    return peeraddr;
}

uint64_t SocketFunc::hostToNetwork64(uint64_t host64)
{
    return htobe64(host64);
}

uint32_t SocketFunc::hostToNetwork32(uint32_t host32)
{
    return htobe32(host32);
}

uint16_t SocketFunc::hostToNetwork16(uint16_t host16)
{
    return htobe16(host16);
}

uint32_t SocketFunc::networkToHost32(uint32_t net32)
{
    return be32toh(net32);
}

uint64_t SocketFunc::networkToHost64(uint64_t net64)
{
    return be64toh(net64);
}

uint16_t SocketFunc::networkToHost16(uint16_t net16)
{
    return be16toh(net16);
}

}

}
