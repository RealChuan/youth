#include "Thread.h"
#include "LogOut.h"
#include "../core/Timestamp.h"

#include <assert.h>

using namespace youth;

Thread::Thread(ThreadFunc func_)
    : running(false),
	  joined(false),
	  pthreadId(0),
	  func(std::move(func_))
{
}

Thread::~Thread()
{
    if (running && !joined)
		pthread_detach(pthreadId);
}

void Thread::start()
{
    assert(!running);
    running = true;
	if (pthread_create(&pthreadId, nullptr, threadFunc, reinterpret_cast<void*>(this))){
        running = false;
		LOG_FATAL << "Failed in pthread_create";
		perror("Failed in pthread_create");
		exit(-1);
	}
}

int Thread::join()
{
    assert(running);
	assert(!joined);
	joined = true;
	return pthread_join(pthreadId, nullptr);
}

//-----------------------------------------------------------

Mutex::Mutex()
{
	pthread_mutex_init(&mutex, nullptr);
}

Mutex::~Mutex()
{
	pthread_mutex_destroy(&mutex);
}

void Mutex::lock()
{
	pthread_mutex_lock(&mutex);
}

void Mutex::unlock()
{
	pthread_mutex_unlock(&mutex);
}

pthread_mutex_t* Mutex::getMutex()
{
	return &mutex;
}

//-------------------------------------------------------

MutexLock::MutexLock(Mutex& mutex_)
	: mutex(mutex_)
{
	mutex.lock();
}

MutexLock::~MutexLock()
{
	mutex.unlock();
}
//-----------------------------------------------------------

Condition::Condition(Mutex& mutex)
	: mutex(mutex)
{
	pthread_cond_init(&cond, nullptr);
}

Condition::~Condition()
{
	pthread_cond_destroy(&cond);
}

void Condition::wait()
{
	pthread_cond_wait(&cond, mutex.getMutex());
}

void Condition::notify()
{
	pthread_cond_signal(&cond);
}

void Condition::notifyAll()
{
	pthread_cond_broadcast(&cond);
}

bool Condition::waitForSeconds(double seconds)
{
	struct timespec abstime;
	// FIXME: use CLOCK_MONOTONIC or CLOCK_MONOTONIC_RAW to prevent time rewind.
	clock_gettime(CLOCK_REALTIME, &abstime);

	const int64_t kNanoSecondsPerSecond = 1000000000;
	int64_t nanoseconds = static_cast<int64_t> (seconds * kNanoSecondsPerSecond);

	abstime.tv_sec += static_cast<time_t> ((abstime.tv_nsec + nanoseconds) / kNanoSecondsPerSecond);
	abstime.tv_nsec = static_cast<long> ((abstime.tv_nsec + nanoseconds) % kNanoSecondsPerSecond);

	return ETIMEDOUT == pthread_cond_timedwait(&cond, mutex.getMutex(), &abstime);
}
