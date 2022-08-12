#include <youth/core/Thread.hpp>

using namespace youth::core;

class A {
public:
  A() : mutex(), condition() {}

  void start() {
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

  void first() {
    printf("This is first.\n");
    MutexLock lock(mutex);
    condition.wait(lock);
    printf("This is first wake.\n");
  }

  void second() {
    printf("This is second.\n");
    MutexLock lock(mutex);
    condition.wait(lock);
    printf("This is second wake.\n");
  }

  void third() {
    printf("This is third.\n");
    std::this_thread::sleep_for(std::chrono::seconds(2));
    printf("This is third again.\n");
    condition.notify_all();
  }

private:
  Mutex mutex;
  Condition condition;
};

int main() {
  A a;
  a.start();
}
