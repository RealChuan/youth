#include <youth/core/Thread.hpp>
#include <youth/utils/Logging.h>

using namespace youth::core;
using namespace youth::utils;

void print()
{
    for (int i = 0; i < 1000; i++) {
        LOG_INFO << i << " "
                 << "1234567890qwertyuiopasdfghjklzxcvbnm";
    }
}

int main(int argc, char **argv)
{
    auto t1 = Thread(print);
    auto t2 = Thread(print);
    auto t3 = Thread(print);

    t1.start();
    t2.start();
    t3.start();

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
