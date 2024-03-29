#include <youth/utils/Logging.h>

using namespace youth::utils;

int main(int argc, char **argv)
{
    Logging::setDirectoryAndBaseName("/tmp/Youth/", argv[0]); // must

    //日志的报警等级为DEBUG
    Logging::setLogLevel(Logging::DEBUG);

    //我们设置日志输出到文件和标准输出
    Logging::setOutputMode(LOGGER_MODE_OUTANDFILE);

    for (int i = 0; i < 1 * 10; i++) {
        LOG_INFO << i;
    }

    return 0;
}
