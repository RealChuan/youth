#ifndef SOCKETFUNC_H
#define SOCKETFUNC_H

#include <string>
#include <netinet/in.h>

namespace youth
{

namespace SocketFunc
{

void setServerAddress(uint16_t port, struct sockaddr_in* serverAddr,
                      bool loopbackOnly = false);
void setServerAddress(uint16_t port, struct sockaddr_in6* serverAddr6,
                      bool loopbackOnly = false);
void setServerAddress(const char* ip, uint16_t port, struct sockaddr_in* serverAddr);
void setServerAddress(const char* ip, uint16_t port, struct sockaddr_in6* serverAddr6);

int socket();

void bind(const int fd, const struct sockaddr *serverAddr);

void listen(const int serverfd, const struct sockaddr *serverAddr);

int accept(const int serverfd, const struct sockaddr_in6 *clientAddr);

int connect(int sockfd, const struct sockaddr* addr);

void shutdownWrite(int sockfd);

void closeSockfd(int sockfd);

std::string getIp(const struct sockaddr *addr);

uint16_t getPort(const struct sockaddr* addr);

std::string getIpAndPort(const struct sockaddr *addr);

ssize_t read(int sockfd, void *buf, size_t count);

ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt);

ssize_t write(int sockfd, const void *buf, size_t count);

}; // namespace socket

}

#endif // SOCKETFUNC_H
