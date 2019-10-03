#include "../Youth/YThreadPool.h"
#include "../Youth/LogOut.h"

using namespace std;
using namespace youth;

//打印带参数
void printBuf(char* buf)
{
	for (int i = 0; i < 1000; i++)
	{
		LOG_INFO << i << buf;
	}
}

//线程池
void threadPoolTest(int maxTaskNum)
{
	YThreadPool threadPool;

	//设置任务队列最大数量
	threadPool.setTaskNum(maxTaskNum);

	//开始跑5条线程
	threadPool.start(10);

	//放入任务
	//threadPool.run(print);

	for (int i = 0; i < 100; i++)
	{
		char buf[32];
		snprintf(buf, sizeof buf, "task %d", i);
		threadPool.run(std::bind(printBuf, buf));
	}

	//缓冲
	//sleep(1);

	//threadPool.stop();
}

int main(int argc, char** argv)
{
	threadPoolTest(5);

	return 0;
}
