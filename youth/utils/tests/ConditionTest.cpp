#include "youth/utils/Thread.h"

#include <unistd.h>

using namespace youth;

class A{
public:
    A()
        :mutex()
        ,condition(mutex)
    {}

    void start(){
        Thread t1(std::bind(&A::first, this));
        Thread t2(std::bind(&A::second, this));
        Thread t3(std::bind(&A::third, this));
        t1.start();
        t2.start();
        t3.start();
        t1.join();
        t2.join();
        t3.join();
    }

    void first(){
        printf("This is first.\n");
        MutexLock lock(mutex);
        condition.wait();
        printf("This is first wake.\n");
    }

    void second(){
        printf("This is second.\n");
        MutexLock lock(mutex);
        condition.wait();
        printf("This is second wake.\n");
    }

    void third(){
        printf("This is third.\n");
        sleep(2);
        printf("This is third again.\n");
        condition.notifyAll();
    }

private:
    Mutex mutex;
    Condition condition GUARDED_BY(mutex);
};

int main()
{
    A a;
    a.start();
}
