#ifndef CURRENTTHREAD_H
#define CURRENTTHREAD_H

#include <unistd.h>
#include <stdint.h>

namespace youth {

namespace CurrentThread{
// internal
extern __thread int tidCache;
extern __thread char tidString_[32];
extern __thread int tidStringLength_;

void cacheTid();

inline int tid(){
    if (__builtin_expect(tidCache == 0, 0))
        cacheTid();
    return tidCache;
}

inline const char* tidString() // for logging
{
    return tidString_;
}

inline int tidStringLength() // for logging
{
    return tidStringLength_;
}

bool isMainThread();

void sleepUsec(int64_t usec);  // for testing

}

}

#endif // CURRENTTHREAD_H
