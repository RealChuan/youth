#include <youth/core/ElapsedTimer.h>

#include <iostream>

using namespace youth::core;

int main()
{
    ElapsedTimer timer;
    std::cout << "begin: " << timer.isVaid() << std::endl;
    timer.start();
    while (1) {
        if (timer.hasExpired(1000 * 1000)) {
            std::cout << "time out: " << timer.elapsed() << std::endl;
            break;
        }
    }
    for (int i = 0; i < 1000; i++) {
        std::cout << "1234567890qwertyuiopasdfghjklzxcvbnm " << i << std::endl;
    }
    std::cout << timer.elapsed() << std::endl;
    std::cout << "end: " << timer.isVaid() << std::endl;
    return 0;
}