#ifndef CURRENTTHREAD_H
#define CURRENTTHREAD_H

#include <stdint.h>
#include <unistd.h>

namespace youth {

namespace core {

namespace CurrentThread {
// internal
extern __thread int tidCache;
extern __thread char tidString_[32];
extern __thread int tidStringLength_;

void cacheTid();

inline int tid()
{
    if (__builtin_expect(tidCache == 0, 0))
        cacheTid();
    return tidCache;
}

inline const char *tidString() // for logging
{
    return tidString_;
}

inline int tidStringLength() // for logging
{
    return tidStringLength_;
}

inline bool isMainThread()
{
    return tid() == ::getpid();
}

} // namespace CurrentThread

} // namespace core

} // namespace youth

#endif // CURRENTTHREAD_H
