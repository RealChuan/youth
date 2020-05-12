#include "TcpClient.h"
#include "Socket.h"

using namespace youth;

TcpClient::TcpClient(const char* ip_, uint16_t port_)
:port(port_)
,ip(ip_)
{
	serverAddr = Socket::addrClient(ip, port);
	clientFd = Socket::socket();
	Socket::connectServer(clientFd, serverAddr);
}

TcpClient::~TcpClient()
{
	Socket::closeSockfd(clientFd);
}

