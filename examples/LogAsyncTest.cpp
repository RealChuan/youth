#include "../Youth/LogAsync.h"
#include "../Youth/LogOut.h"

using namespace std;
using namespace youth;

//打印
void print()
{
	for (int i = 0; i < 1000; i++)
	{
		LOG_INFO << i;
	}
}

//线程日志
void LogAsyncTest(const char* basename)
{
	//设置日志名字
	Logging::setFileBaseName(basename);

	//日志的报警等级为DEBUG
	Logging::setLogLevel(Logging::DEBUG);

	//异步模式设置
	LogAsync m_LogAsync(3); //3秒打印一次

	m_LogAsync.start();

	print();

	//缓冲时间
	sleep(2);

	//m_LogAsync.stop();
}

int main(int argc, char** argv)
{
	LogAsyncTest(argv[0]);
	return 0;
}
