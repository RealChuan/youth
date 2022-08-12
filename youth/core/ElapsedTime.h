#ifndef ELAPSEDTIME_H
#define ELAPSEDTIME_H

#include <chrono>
#include <string>

#include "Object.h"

namespace youth {

namespace core {

class ElapsedTime : noncopyable {
public:
  ElapsedTime();

  void start();
  void reStart();
  std::string elapsed() const;

private:
  std::chrono::steady_clock::time_point m_startPoint;
};

} // namespace core

} // namespace youth

#endif // ELAPSEDTIME_H
