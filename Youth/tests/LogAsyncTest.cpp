#include "../LogAsync.h"
#include "../LogOut.h"

#include <unistd.h>

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

int main(int argc, char** argv)
{
    //设置日志名字
    Logging::setFileBaseName(argv[0]);

    //日志的报警等级为DEBUG
    Logging::setLogLevel(Logging::DEBUG);

    //异步模式设置
    LogAsync m_LogAsync(3); //3秒打印一次

    m_LogAsync.start();

    for (int i = 0; i < 1000 * 1000; i++)
        LOG_INFO << i;

    //缓冲时间
    sleep(20);

    //m_LogAsync.stop();
    return 0;
}
