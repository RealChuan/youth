#include <youth/core/Thread.hpp>
#include <youth/utils/Logging.h>
#include <youth/utils/Singleton.h>

#include <gtest/gtest.h>

using namespace youth::utils;
using namespace youth::core;

class Sing
{
    SINGLETON(Sing)

    bool m_state = false;

public:
    void print() { LOG_INFO << "Hello World!"; }
};

Sing::Sing()
{
    EXPECT_FALSE(m_state);
    m_state = true;
    LOG_INFO << "Sing";
}
Sing::~Sing()
{
    EXPECT_TRUE(m_state);
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
