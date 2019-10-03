#include "../TcpServer.h"
#include"../LogOut.h"

#include "signal.h"

using namespace youth;

void quitFunc(int code)
{
	LOG_INFO<<"程序退出!\n"<<"code="<<code;
	exit(0);
}

void readfd(int fd)
{
	ssize_t size=0;
	char recvbuf[1024];
	size=recv(fd,recvbuf,sizeof recvbuf,0);
	LOG_INFO<<size;
	recvbuf[size]='\0';
	if(size>0)
		LOG_INFO<<recvbuf;
	const char* sendbuf = "Hello Client!";
	send(fd, sendbuf, 13, 0);
}

void tcpServer(uint16_t port)
{
	TcpServer server(port,readfd);
	server.Loop();
}

int main(int argc, char** argv)
{
	signal(SIGINT, quitFunc);
	tcpServer(6666);

	return 0;
}
