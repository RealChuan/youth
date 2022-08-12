#include "ElapsedTime.h"
#include "Timestamp.h"

namespace youth {

namespace core {

ElapsedTime::ElapsedTime() { start(); }

void ElapsedTime::start() { m_startPoint = std::chrono::steady_clock::now(); }

void ElapsedTime::reStart() { start(); }

std::string ElapsedTime::elapsed() const {
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - m_startPoint;
  char buf[20];
  snprintf(buf, sizeof buf, "%.6lf(S)", elapsed_seconds.count());
  return buf;
}

} // namespace core

} // namespace youth
