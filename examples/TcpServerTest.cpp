#include "../Youth/TcpServer.h"

using namespace std;
using namespace youth;

void tcpServer(uint16_t port)
{
	TcpServer server(port);
	const char* buf = "hello world";
	send(server.acceptFd, buf, 1024, 0);
	//while (1);
}

int main(int argc, char** argv)
{
	tcpServer(6666);

	return 0;
}
