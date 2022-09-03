#pragma once

#include <youth/core/Object.h>

#include <memory>

namespace google_breakpad {
class ExceptionHandler;
}

namespace youth {

namespace utils {

class BreakPad : core::noncopyable
{
public:
    BreakPad(const std::string &dumpPath);
    ~BreakPad();

private:
    std::unique_ptr<google_breakpad::ExceptionHandler> m_exceptionHandlerPtr;
};

} // namespace utils

} // namespace youth
