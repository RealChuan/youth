#include <sys/epoll.h>

#include "Epoll.h"
#include "Socket.h"
#include "youth/utils/LogOut.h"

using namespace youth;

Epoll::Epoll()
{

}

Epoll::~Epoll()
{

}

Timestamp Epoll::poll(int timeoutMs)
{
    int numEvents = ::epoll_wait(m_epollfd, &*m_events.begin(),
                                 static_cast<int>(m_events.size()),
                                 timeoutMs);
    int savedErrno = errno;
    Timestamp timestamp(Timestamp::currentTimestamp());
    if (numEvents > 0)
    {
        LOG_DEBUG << numEvents << " events happened";
        //Todo
    }
}
