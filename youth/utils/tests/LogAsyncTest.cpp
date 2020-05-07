#include "../LogAsync.h"
#include "../LogOut.h"

#include <unistd.h>

using namespace std;
using namespace youth;

int main(int argc, char** argv)
{
    //设置日志名字
    Logging::setFileBaseName(argv[0]);

    //异步模式设置
    LogAsync logAsync(3); //3秒打印一次

    logAsync.start();

    for (int i = 0; i < 1000 * 1000; i++)
        LOG_INFO << "1234567890qwertyuiopasdfghjklzxcvbnm" << i;

    logAsync.stop();
    return 0;
}
