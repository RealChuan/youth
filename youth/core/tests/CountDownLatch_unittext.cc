#include <youth/core/CountDownLatch.h>
#include <youth/core/Thread.hpp>

#include <memory>
#include <sstream>

using namespace youth::core;

// std::cout is not Thread safe

std::string getThisThreadIdString()
{
    std::ostringstream oss;
    oss << std::this_thread::get_id();
    return oss.str();
}

void printFirst(CountDownLatch *latch)
{
    latch->wait();
    printf("This is the first thread: %s.\n", getThisThreadIdString().c_str());
}

void printSecond(CountDownLatch *latch)
{
    latch->wait();
    printf("This is the Second thread: %s.\n", getThisThreadIdString().c_str());
}

void printThird(CountDownLatch *latch)
{
    latch->wait();
    printf("This is the Third thread: %s.\n", getThisThreadIdString().c_str());
}

int main()
{
    std::unique_ptr<CountDownLatch> latch(new CountDownLatch(1));
    Thread t1(std::bind(printFirst, latch.get()));
    Thread t2(std::bind(printSecond, latch.get()));
    Thread t3(std::bind(printThird, latch.get()));
    t1.start();
    t2.start();
    t3.start();
    printf("This is main thread.\n");
    latch->countDown();
    t1.join();
    t2.join();
    t3.join();
}
