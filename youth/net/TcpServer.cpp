#include "TcpServer.h"
#include "Socket.h"
#include "Epoll.h"

#include "../utils/LogOut.h"

using namespace youth;

TcpServer::TcpServer(uint16_t port_,callBackFunc readCallBackFunc_)
	: port(port_)
	,readCallBackFunc(std::move(readCallBackFunc_))
{
	serverAddr = Socket::addrServer(port);
	serverFd = Socket::socket();
	Socket::listenAndBindServer(serverFd, serverAddr);
}

TcpServer::~TcpServer()
{
	Socket::closeSock(serverFd);
}

void TcpServer::Loop()
{
	Epoll myEpoll(serverFd,readCallBackFunc);
	myEpoll.Loop_1();
}

