#include "Acceptor.h"
#include "Channel.h"
#include "EventLoop.h"
#include "HostAddress.hpp"
#include "Socket.h"
#include "SocketFunc.h"

#include <youth/utils/Logging.h>

#include <assert.h>
#include <fcntl.h>

namespace youth {

using namespace utils;

namespace net {

Acceptor::Acceptor(EventLoop *loop, const HostAddress &listenAddr, bool reuseport)
    : m_eventLoop(loop)
    , m_acceptSocketPtr(new Socket(SocketFunc::createNonblockingOrDie(listenAddr.family())))
    , m_acceptChannelPtr(new Channel(loop, m_acceptSocketPtr->fd()))
    , m_listenning(false)
    , m_idleFd(::open("/dev/null", O_RDONLY | O_CLOEXEC))

{
    assert(m_idleFd >= 0);
    m_acceptSocketPtr->setReuseAddr(true);
    m_acceptSocketPtr->setReusePort(reuseport);
    m_acceptSocketPtr->bindAddress(listenAddr);
    m_acceptChannelPtr->setReadCallback(std::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor()
{
    m_acceptChannelPtr->disableAll();
    m_acceptChannelPtr->remove();
    SocketFunc::close(m_idleFd);
}

void Acceptor::listen()
{
    m_eventLoop->assertInLoopThread();
    m_listenning = true;
    m_acceptSocketPtr->listen();
    m_acceptChannelPtr->enableReading();
}

void Acceptor::handleRead()
{
    m_eventLoop->assertInLoopThread();
    HostAddress peerAddr;
    //FIXME loop until no more
    int connfd = m_acceptSocketPtr->accept(&peerAddr);
    if (connfd >= 0) {
        // string hostport = peerAddr.toIpPort();
        // LOG_TRACE << "Accepts of " << hostport;
        if (m_newConnectionCallback) {
            m_newConnectionCallback(connfd, peerAddr);
        } else {
            SocketFunc::close(connfd);
        }
    } else {
        LOG_ERROR << "in Acceptor::handleRead";
        // Read the section named "The special problem of
        // accept()ing when you can't" in libev's doc.
        // By Marc Lehmann, author of libev.
        if (errno == EMFILE) {
            SocketFunc::close(m_idleFd);
            m_idleFd = ::accept(m_acceptSocketPtr->fd(), NULL, NULL);
            SocketFunc::close(m_idleFd);
            m_idleFd = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
        }
    }
}

} // namespace net

} // namespace youth
