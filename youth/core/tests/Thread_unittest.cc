#include <youth/core/Thread.hpp>

#include <gtest/gtest.h>

#include <iostream>
#include <sstream>

using namespace youth::core;

void test(const std::string &name)
{
    std::ostringstream oss;
    oss << Thread::currentThreadId();
    printf("%s Testing thread: %s.\n", name.c_str(), oss.str().c_str());
    Thread::sleep(1);
}

TEST(threadTest, void)
{
    auto t1 = Thread([] { test("t1"); }, "t1");
    auto t2 = Thread([] { test("t2"); }, "t2");
    auto t3 = Thread([] { test("t3"); }, "t3");
    t1.start();
    t2.start();
    t3.start();
    std::cout << "t1: " << t1.threadId() << " " << t1.name() << std::endl;
    std::cout << "t2: " << t2.threadId() << " " << t2.name() << std::endl;
    std::cout << "t3: " << t3.threadId() << " " << t3.name() << std::endl;
    EXPECT_TRUE(t1.isRunning());
    EXPECT_TRUE(t2.isRunning());
    EXPECT_TRUE(t3.isRunning());
    EXPECT_EQ("t1", t1.name());
    EXPECT_EQ("t2", t2.name());
    EXPECT_EQ("t3", t3.name());
    EXPECT_EQ(Thread::numCreated(), 3);
    EXPECT_EQ(Thread::allThreadNum(), 3);
    t1.join();
    t2.join();
    t3.join();
    EXPECT_EQ(Thread::numCreated(), 3);
    EXPECT_EQ(Thread::allThreadNum(), 0);
}
