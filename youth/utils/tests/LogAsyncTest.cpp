#include <youth/utils/LogAsync.h>
#include <youth/utils/Logging.h>

#include <unistd.h>

using namespace youth::utils;

int main(int argc, char** argv)
{
    //设置日志名字
    Logging::setFileBaseName(argv[0]);

    //异步模式设置
    LogAsync *logAsync = LogAsync::instance();
    logAsync->setFlushInterval(3); //3秒打印一次
    logAsync->start();

    for (int i = 0; i < 1000 * 1000; i++)
        LOG_INFO << "1234567890qwertyuiopasdfghjklzxcvbnm" << i;

    logAsync->stop();
    return 0;
}
