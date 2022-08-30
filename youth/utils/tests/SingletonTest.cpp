#include <youth/core/Thread.hpp>
#include <youth/utils/Logging.h>
#include <youth/utils/Singleton.h>

#include <gtest/gtest.h>

using namespace youth::utils;
using namespace youth::core;

static std::atomic_int64_t s_num = 0;

class Sing
{
    SINGLETON(Sing)

public:
    void print() { LOG_INFO << "Hello World!"; }
};

Sing::Sing()
{
    EXPECT_EQ(s_num.load(), 0);
    s_num++;
    LOG_INFO << "Sing";
}
Sing::~Sing()
{
    EXPECT_EQ(s_num.load(), 1);
    LOG_INFO << "~Sing";
}

TEST(SingletonTest, Singleton)
{
    Thread thread1([] { Sing::instance().print(); });
    Thread thread2([] { Sing::instance().print(); });
    Thread thread3([] { Sing::instance().print(); });

    thread1.start();
    thread2.start();
    thread3.start();

    Sing::instance().print();
    Sing::instance().print();
    Sing::instance().print();
    Sing::instance().print();

    thread1.join();
    thread2.join();
    thread3.join();
}
