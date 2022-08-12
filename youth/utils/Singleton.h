#ifndef SINGLETON_H
#define SINGLETON_H

#include <assert.h>
#include <memory>

#include <youth/core/Object.h>
#include <youth/core/Thread.hpp>

namespace youth {

using namespace core;

namespace utils {

template <typename T> class Singleton : noncopyable {
public:
  static T &getInstance() {
    static T t; // C++11 thread safe
    return t;
  }

private:
  Singleton() = delete;
  ~Singleton() = delete;
};

#define SINGLETON(Class)                                                       \
private:                                                                       \
  Class();                                                                     \
  ~Class();                                                                    \
  Class(const Class &other) = delete;                                          \
  Class &operator=(const Class &other) = delete;                               \
  Class(Class &&other) = delete;                                               \
  Class &operator=(Class &&other) = delete;                                    \
  friend class Singleton<Class>;                                               \
                                                                               \
public:                                                                        \
  static Class &instance() { return Singleton<Class>::getInstance(); }

} // namespace utils

} // namespace youth

#endif // SINGLETON_H
