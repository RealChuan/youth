#include "Socket.h"
#include "../utils/LogOut.h"

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
//#include <sys/socket.h>

using namespace youth;

struct sockaddr_in Socket::addrServer(uint16_t port)
{
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof (struct sockaddr_in));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	return serverAddr;
}

int Socket::socket()
{
	int fd = 0;
	//fd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
	fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd < 0)
	{
		LOG_FATAL << "socket Server error:" << errno;
	}
	return fd;
}

void Socket::listenAndBindServer(int serverfd, struct sockaddr_in serverAddr)
{
	//bind
	int ret = ::bind(serverfd, reinterpret_cast<struct sockaddr*>(&serverAddr),
					 static_cast<socklen_t> (sizeof (struct sockaddr_in)));
	if (ret < 0)
	{
		LOG_FATAL << "bind error:" << errno;
		return;
	}

	//listen
	int ret_ = ::listen(serverfd, SOMAXCONN);
	if (ret_ < 0)
	{
		LOG_FATAL << "listen error:" << errno;
		return;
	}

	std::cout << "服务器 :" << Socket::getIpAndPort(serverAddr) << "正在监听..." << std::endl;
}

int Socket::acceptServer(int serverfd)
{
	struct sockaddr_in clientAddr;
	LOG_INFO << "等待客户端连接...";
	socklen_t addrlen = static_cast<socklen_t> (sizeof clientAddr);
	int connfd = ::accept4(serverfd, reinterpret_cast<struct sockaddr*>(&clientAddr),
						   &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
	if (connfd < 0)
	{
		int savedErrno = errno;
		LOG_FATAL << "Socket::accept";
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
		return -1;
	}
	LOG_INFO << "客户端上线：" << Socket::getIpAndPort(clientAddr);
	return connfd;
}

struct sockaddr_in Socket::addrClient(const char* ip, uint16_t port)
{
	struct sockaddr_in serverAddr_;
	memset(&serverAddr_, 0, sizeof (struct sockaddr_in));
	//设置服务器的地址
	serverAddr_.sin_family = AF_INET;
	serverAddr_.sin_port = htons(port);
	serverAddr_.sin_addr.s_addr = inet_addr(ip);
	//inet_pton(AF_INET, ip, &clientsock.sin_addr.s_addr);

	LOG_DEBUG << "服务器设置：" << Socket::getIpAndPort(serverAddr_);

	return serverAddr_;
}

void Socket::connectServer(int sockfd, struct sockaddr_in serverAddr_)
{
	int ret = connect(sockfd, reinterpret_cast<struct sockaddr*>(&serverAddr_),
					  static_cast<socklen_t> (sizeof serverAddr_));
	if (ret < 0)
	{
		perror("error:");
		LOG_FATAL << "connect Server error:" << errno;
		Socket::closeSock(sockfd);
		return;
	}

	LOG_INFO << "本地客户端：" << Socket::getLocalIpAndPort(sockfd);
	LOG_INFO << "成功连接上服务端：" << Socket::getIpAndPort(serverAddr_);
}

void Socket::closeSock(int sockfd)
{
	if (sockfd > 0)
		close(sockfd);
}

std::string Socket::getIpAndPort(struct sockaddr_in addr)
{
	char buf[256];
	snprintf(buf, 256, "%s:%d", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
	return buf;
}

std::string Socket::getLocalIpAndPort(int sockfd)
{
	struct sockaddr_in addr;
	socklen_t addrlen = static_cast<socklen_t> (sizeof addr);
	int ret = getsockname(sockfd, reinterpret_cast<struct sockaddr*>(&addr), &addrlen);
	if (ret == 0)
	{
		return Socket::getIpAndPort(addr);
	}
	char buf[10];
	memset(buf, 0, sizeof buf);
	return buf;
}
