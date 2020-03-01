#include "../Thread.h"
#include "../../core/CurrentThread.h"

using namespace youth;

class Add{
public:
    Add()
        :mutex()
        ,count(0)
    {}
    void start(){
        Thread t1(std::bind(&Add::add, this));
        Thread t2(std::bind(&Add::add, this));
        Thread t3(std::bind(&Add::add, this));
        t1.start();
        t2.start();
        t3.start();
        t1.join();
        t2.join();
        t3.join();
    }
    void add(){
        for(int i=0; i<1000; i++){
            MutexLock lock(mutex);
            CurrentThread::cacheTid();
            printf("pid = %s, count = %d\n",CurrentThread::tidString(), count);
            ++count;
        }
    }

private:
    Mutex mutex;
    int count ;//GUARDED_BY(mutex);
};

int main()
{
    Add a;
    a.start();
}
