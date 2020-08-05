#ifndef SOCKET_H
#define SOCKET_H

#include <youth/core/Object.h>

#include <netinet/in.h>
#include <string>

struct tcp_info;

namespace youth
{

class TcpAddressInfo : copyable
{
public:
    explicit TcpAddressInfo(uint16_t port = 0, bool loopbackOnly = false,
                            bool ipv6 = false);

    TcpAddressInfo(const char* ip, uint16_t port = 0, bool ipv6 = false);

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

class Socket : noncopyable
{
public:
    Socket(int sockfd);
    ~Socket();

    int fd() const;

    bool getTcpInfo(struct tcp_info*) const;
    bool getTcpInfoString(char* buf, int len) const;

    void bindAddress(const TcpAddressInfo&);

    void listen(const TcpAddressInfo&);

    /// On success, returns a non-negative integer that is
    /// a descriptor for the accepted socket, which has been
    /// set to non-blocking and close-on-exec. *peeraddr is assigned.
    /// On error, -1 is returned, and *peeraddr is untouched.
    int accept(TcpAddressInfo* peeraddr);

    void shutdownWrite();

    // setsockopt
    ///
    /// Enable/disable TCP_NODELAY (disable/enable Nagle's algorithm).
    ///
    void setTcpNoDelay(bool on);

    ///
    /// Enable/disable SO_REUSEADDR
    ///
    void setReuseAddr(bool on);

    ///
    /// Enable/disable SO_REUSEPORT
    ///
    void setReusePort(bool on);

    ///
    /// Enable/disable SO_KEEPALIVE
    ///
    void setKeepAlive(bool on);

private:
    int m_sockfd;
};

} // namespace youth

#endif /* SOCKET_H */
