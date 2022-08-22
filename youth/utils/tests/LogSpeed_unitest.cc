#include <youth/core/ElapsedTimer.h>
#include <youth/utils/Logging.h>

#include <unistd.h>

using namespace youth::core;
using namespace youth::utils;

void test1000_10()
{
    printf("test1000_10\n");
    ElapsedTimer timer;
    timer.start();
    for (int i = 0; i < 1000 * 10; i++) {
        LOG_INFO << "1234567890qwertyuiopasdfghjklzxcvbnm" << i;
    }
    printf("%s\n", timer.elapsed().c_str());
}

void test1000_100()
{
    printf("test1000_100\n");
    ElapsedTimer timer;
    timer.start();
    for (int i = 0; i < 1000 * 100; i++) {
        LOG_INFO << "1234567890qwertyuiopasdfghjklzxcvbnm" << i;
    }
    printf("%s\n", timer.elapsed().c_str());
}

void test1000_1000()
{
    printf("test1000_1000\n");
    ElapsedTimer timer;
    timer.start();
    for (int i = 0; i < 1000 * 1000; i++) {
        LOG_INFO << "1234567890qwertyuiopasdfghjklzxcvbnm" << i;
    }
    printf("%s\n", timer.elapsed().c_str());
}

void test1000_1000_10()
{
    printf("test1000_1000_10\n");
    ElapsedTimer timer;
    timer.start();
    for (int i = 0; i < 1000 * 1000 * 10; i++) {
        LOG_INFO << "1234567890qwertyuiopasdfghjklzxcvbnm" << i;
    }
    printf("%s\n", timer.elapsed().c_str());
}

int main(int argc, char **argv)
{
    Logging::setFileBaseName(argv[0]);
    Logging::setOutputMode(LOGGER_MODE_LOGFILE);
    test1000_10();
    test1000_100();
    test1000_1000();
    test1000_1000_10();
    return 0;
}
