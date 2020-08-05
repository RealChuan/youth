#include "Thread.h"
#include "LogOut.h"
#include "../core/Timestamp.h"

#include <assert.h>

using namespace youth;

Thread::Thread(ThreadFunc func_)
    : m_running(false),
	  m_joined(false),
	  m_pthreadId(0),
	  m_func(std::move(func_))
{
}

Thread::~Thread()
{
    if (m_running && !m_joined)
		pthread_detach(m_pthreadId);
}

void Thread::start()
{
    assert(!m_running);
    m_running = true;
	if (pthread_create(&m_pthreadId, nullptr, threadFunc, reinterpret_cast<void*>(this))){
        m_running = false;
		LOG_FATAL << "Failed in pthread_create";
		perror("Failed in pthread_create");
		exit(-1);
	}
}

int Thread::join()
{
    assert(m_running);
	assert(!m_joined);
	m_joined = true;
	return pthread_join(m_pthreadId, nullptr);
}

//-----------------------------------------------------------

Mutex::Mutex()
{
	pthread_mutex_init(&m_mutex, nullptr);
}

Mutex::~Mutex()
{
	pthread_mutex_destroy(&m_mutex);
}

void Mutex::lock()
{
	pthread_mutex_lock(&m_mutex);
}

void Mutex::unlock()
{
	pthread_mutex_unlock(&m_mutex);
}

pthread_mutex_t* Mutex::getMutex()
{
	return &m_mutex;
}

//-------------------------------------------------------

MutexLock::MutexLock(Mutex& mutex_)
	: m_mutex(mutex_)
{
	m_mutex.lock();
}

MutexLock::~MutexLock()
{
	m_mutex.unlock();
}
//-----------------------------------------------------------

Condition::Condition(Mutex& mutex)
    : m_mutex(mutex)
{
    pthread_cond_init(&m_cond, nullptr);
}

Condition::~Condition()
{
    pthread_cond_destroy(&m_cond);
}

void Condition::wait()
{
    pthread_cond_wait(&m_cond, m_mutex.getMutex());
}

void Condition::notify()
{
    pthread_cond_signal(&m_cond);
}

void Condition::notifyAll()
{
    pthread_cond_broadcast(&m_cond);
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

    return ETIMEDOUT == pthread_cond_timedwait(&m_cond, m_mutex.getMutex(), &abstime);
}
