#include "SocketFunc.h"
#include "youth/utils/LogOut.h"

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/uio.h>

using namespace youth;

void SocketFunc::setServerAddress(uint16_t port, sockaddr_in *serverAddr,
                                  bool loopbackOnly)
{
    memset(serverAddr, 0, sizeof(struct sockaddr_in));
    serverAddr->sin_family = AF_INET;
    serverAddr->sin_port = htobe16(port);
    in_addr_t ip = loopbackOnly ? INADDR_LOOPBACK : INADDR_ANY;
    serverAddr->sin_addr.s_addr = htobe32(ip);
}

void SocketFunc::setServerAddress(uint16_t port, sockaddr_in6 *serverAddr6,
                                  bool loopbackOnly)
{
    memset(serverAddr6, 0, sizeof(struct sockaddr_in));
    serverAddr6->sin6_family = AF_INET6;
    serverAddr6->sin6_port = htobe16(port);
    in6_addr ip = loopbackOnly ? in6addr_loopback : in6addr_any;
    serverAddr6->sin6_addr = ip;
}

void SocketFunc::setServerAddress(const char *ip, uint16_t port,
                                  sockaddr_in *serverAddr)
{
    memset(serverAddr, 0, sizeof(struct sockaddr_in));
    serverAddr->sin_family = AF_INET;
    serverAddr->sin_port = htobe16(port);
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
    serverAddr6->sin6_port = htobe16(port);
    if (::inet_pton(AF_INET6, ip, &serverAddr6->sin6_addr) <= 0)
    {
        LOG_ERROR << "SocketFunc::serServerAddr";
    }
}

int SocketFunc::socket()
{
#if VALGRIND
    int sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
    {
        LOG_FATAL << "sockets::createNonblockingOrDie";
    }

    setNonBlockAndCloseOnExec(sockfd);
#else
    int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if (sockfd < 0)
    {
        LOG_FATAL << "sockets::createNonblockingOrDie";
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
        LOG_ERROR << "Socket::accept";
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
        LOG_ERROR << "sockets::shutdownWrite";
    }
}

void SocketFunc::closeSockfd(int sockfd)
{
    if (sockfd > 0)
    {
        if (::close(sockfd) < 0)
        {
            LOG_ERROR << "sockets::close";
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
