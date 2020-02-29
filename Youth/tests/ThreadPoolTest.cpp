#include "../ThreadPool.h"
#include "../LogOut.h"

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

int main(int argc, char** argv)
{
    ThreadPool threadPool;

    //设置任务队列最大数量
    threadPool.setTaskNum(5);

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

    return 0;
}
