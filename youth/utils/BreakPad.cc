#include "BreakPad.hpp"
#include "Logging.h"

#include <client/linux/handler/exception_handler.h>

namespace youth {

namespace utils {

bool minidumpCallback(const google_breakpad::MinidumpDescriptor &descriptor,
                      void *context,
                      bool succeeded)
{
    if (succeeded) {
        LOG_INFO << "Create dump file success:" << descriptor.path();
    } else {
        LOG_WARN << "Create dump file failed";
    }
    return succeeded;
}

BreakPad::BreakPad(const std::string &dumpPath)
    : m_exceptionHandlerPtr(
        new google_breakpad::ExceptionHandler(google_breakpad::MinidumpDescriptor(dumpPath),
                                              nullptr,
                                              minidumpCallback,
                                              nullptr,
                                              true,
                                              -1))
{}

BreakPad::~BreakPad() {}

} // namespace utils

} // namespace youth
