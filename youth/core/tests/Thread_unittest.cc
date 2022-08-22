#include <youth/core/Thread.hpp>

#include <iostream>
#include <sstream>

using namespace youth::core;

void test(const std::string &name)
{
    std::ostringstream oss;
    oss << Thread::currentThreadId();
    printf("%s Testing thread: %s.\n", name.c_str(), oss.str().c_str());
}

int main()
{
    auto t1 = Thread([] { test("t1"); });
    auto t2 = Thread([] { test("t2"); });
    auto t3 = Thread([] { test("t3"); });
    t1.start();
    t2.start();
    t3.start();
    std::cout << "t1: " << t1.threadId() << " " << t1.name() << std::endl;
    std::cout << "t2: " << t2.threadId() << " " << t2.name() << std::endl;
    std::cout << "t3: " << t3.threadId() << " " << t3.name() << std::endl;
    std::cout << "numCreated: " << Thread::numCreated() << std::endl;
    std::cout << "allThreadNum: " << Thread::allThreadNum() << std::endl;
    t1.join();
    t2.join();
    t3.join();
    std::cout << "numCreated: " << Thread::numCreated() << std::endl;
    std::cout << "allThreadNum: " << Thread::allThreadNum() << std::endl;
    return 0;
}
