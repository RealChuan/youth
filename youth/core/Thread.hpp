#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

#include "Object.h"

namespace youth {

namespace core {

using Mutex = std::mutex;

using MutexLock = std::unique_lock<Mutex>;

using Condition = std::condition_variable;

class Thread : noncopyable {
  using ThreadFunc = std::function<void()>;

public:
  explicit Thread(ThreadFunc threadFunc,
                  const std::string &name = std::string());
  ~Thread();

  void start();
  void join();

  bool isRunning() const { return m_running; }

  std::thread::id threadId() const { return m_thread.get_id(); }
  pid_t tid() const { return m_tid; }
  std::string name() const { return m_name; }

  static int64_t numCreated() { return s_numCreated; }
  static int64_t allThreadNum() { return s_allThreadNum; }

private:
  void threadFunc();
  void setDefaultName();

  std::thread m_thread;

  bool m_running = false;
  bool m_joined = false;
  pid_t m_tid;
  ThreadFunc m_func;
  std::string m_name;

  static std::atomic<int64_t> s_numCreated;
  static std::atomic<int64_t> s_allThreadNum;
};

} // namespace core

} // namespace youth
