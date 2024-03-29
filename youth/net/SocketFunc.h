#ifndef SOCKETFUNC_H
#define SOCKETFUNC_H

#include <netinet/in.h>
#include <string>

namespace youth {

namespace net {

namespace SocketFunc {

void setServerAddress(uint16_t port, struct sockaddr_in *serverAddr, bool loopbackOnly = false);
void setServerAddress(uint16_t port, struct sockaddr_in6 *serverAddr6, bool loopbackOnly = false);
void setServerAddress(const char *ip, uint16_t port, struct sockaddr_in *serverAddr);
void setServerAddress(const char *ip, uint16_t port, struct sockaddr_in6 *serverAddr6);

int createNonblockingOrDie(sa_family_t family);
void bind(const int fd, const struct sockaddr *serverAddr);
void listen(const int serverfd);
int accept(const int serverfd, struct sockaddr_in6 *clientAddr);
int connect(int sockfd, const struct sockaddr *addr);
void shutdownWrite(int sockfd);
void close(int sockfd);

std::string getIp(const struct sockaddr *addr);
uint16_t getPort(const struct sockaddr *addr);
std::string getIpAndPort(const struct sockaddr *addr);

ssize_t read(int sockfd, void *buf, size_t count);
ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt);
ssize_t write(int sockfd, const void *buf, size_t count);

struct sockaddr_in6 getLocalAddr(int sockfd);
struct sockaddr_in6 getPeerAddr(int sockfd);

bool isSelfConnect(int sockfd);
int getSocketError(int sockfd);

uint64_t hostToNetwork64(uint64_t host64);
uint32_t hostToNetwork32(uint32_t host32);
uint16_t hostToNetwork16(uint16_t host16);
uint64_t networkToHost64(uint64_t net64);
uint32_t networkToHost32(uint32_t net32);
uint16_t networkToHost16(uint16_t net16);

} // namespace SocketFunc

} // namespace net

} // namespace youth

#endif // SOCKETFUNC_H
