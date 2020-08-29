#include "ProcessMsg.h"

#include <unistd.h>
#include <string.h>
#include <sys/syscall.h> 

namespace youth
{

namespace core
{





std::string ProcessMsg::hostname()
{
    // HOST_NAME_MAX 64
    // _POSIX_HOST_NAME_MAX 255
    char buf[256];
    if (::gethostname(buf, sizeof buf) == 0)
    {
        buf[sizeof (buf) - 1] = '\0';
        return buf;
    }
    else
        return "unknownhost";
}

std::string ProcessMsg::fileBaseName(const char* basename)
{
    const char *pTmp = strrchr(basename, '/');
    std::string _basename;
    //int Len = 0;

    if (pTmp != nullptr)
    {
        _basename += pTmp + 1;
        //Len = static_cast<int> (strlen(_basename));
    }
    return _basename;
}

pid_t ProcessMsg::getTid()
{
    return pid_t(::syscall(SYS_gettid));
}

pid_t ProcessMsg::getPid()
{
    return ::getpid();
}

}

}
