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

//常规日志
void LogTest(const char* basename)
{
	//设置日志名字
	Logging::setFileBaseName(basename);

	//日志的报警等级为DEBUG
	Logging::setLogLevel(Logging::DEBUG);

	//我们设置日志输出到文件和标准输出
	Logging::setOutputMode(LOGGER_MODE_OUTANDFILE);

	print();
}

int main(int argc, char** argv)
{
	LogTest(argv[0]);

	return 0;
}
