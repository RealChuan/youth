#include "../CountDownLatch.h"
#include "../Thread.h"
#include "iostream"

using namespace youth;

void printFirst(CountDownLatch *latch){
    latch->wait();
    printf("This is the first thread.\n");
}

void printSecond(CountDownLatch *latch){
    latch->wait();
    printf( "This is the Second thread.\n");
}

void printThird(CountDownLatch *latch){
    latch->wait();
    printf("This is the Third thread.\n");
}

int main()
{
    std::shared_ptr<CountDownLatch> latch(new CountDownLatch(1));
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
