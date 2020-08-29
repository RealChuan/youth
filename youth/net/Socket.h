#ifndef SOCKET_H
#define SOCKET_H

#include <youth/core/Object.h>

struct tcp_info;

namespace youth
{

using namespace core;

namespace net
{

class TcpAddressInfo;
class Socket : noncopyable
{
public:
    Socket(int sockfd);
    ~Socket();

    int fd() const { return m_sockfd; }

    bool getTcpInfo(struct tcp_info*) const;
    bool getTcpInfoString(char* buf, int len) const;

    void bindAddress(const TcpAddressInfo&);

    void listen();

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

}

} // namespace youth

#endif /* SOCKET_H */
