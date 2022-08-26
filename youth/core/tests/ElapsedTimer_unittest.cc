#include <youth/core/ElapsedTimer.h>

#include <gtest/gtest.h>

#include <iostream>

using namespace youth::core;

TEST(ElapsedTimerTest, void)
{
    ElapsedTimer timer;
    EXPECT_FALSE(timer.isVaid());
    timer.start();
    EXPECT_TRUE(timer.isVaid());
    while (!timer.hasExpired(1000 * 1000)) {}
    std::cout << "time out: " << timer.elapsed() << std::endl;
    timer.reStart();
    int sum = 0;
    for (int i = 1; i < 10000; i++) {
        sum += i;
    }
    std::cout << timer.elapsed() << " " << sum << std::endl;
}
