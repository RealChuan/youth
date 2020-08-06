#include <youth/core/Mutex.h>
#include <youth/core/CurrentThread.h>
#include <youth/utils/Thread.h>

using namespace youth::core;
using namespace youth::utils;

class Add
{
public:
    Add()
        : m_mutex()
        , m_count(0)
    {}

    void start()
    {
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

    void add()
    {
        for(int i=0; i<1000; i++)
        {
            MutexLock lock(m_mutex);
            CurrentThread::cacheTid();
            printf("pid = %s, count = %d\n",CurrentThread::tidString(), m_count);
            ++m_count;
        }
    }

private:
    Mutex m_mutex;
    int m_count ;//GUARDED_BY(mutex);
};

int main()
{
    Add a;
    a.start();
}
