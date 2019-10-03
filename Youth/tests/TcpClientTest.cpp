#include "../LogOut.h"
#include "../TcpClient.h"

using namespace std;
using namespace youth;

void tcpClient(const char* ip, uint16_t port)
{
	//日志的报警等级为DEBUG
	Logging::setLogLevel(Logging::DEBUG);

	TcpClient client(ip, port);
	const char* buf = "hello world";
	send(client.clientFd, buf, 1024, 0);
	//while (1);
}

int main(int argc, char** argv)
{
	tcpClient("192.168.0.106",6666);

	return 0;
}
