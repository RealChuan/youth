#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <youth/core/Object.h>

#include <functional>
#include <memory>

namespace youth {

using namespace core;

namespace net {

class Socket;
class EventLoop;
class Channel;
class HostAddress;
class Acceptor : noncopyable
{
public:
    using NewConnectionCallback = std::function<void(int sockfd, const HostAddress &)>;

    Acceptor(EventLoop *loop, const HostAddress &listenAddr, bool reuseport);
    ~Acceptor();

    void setNewConnectionCallback(const NewConnectionCallback &cb) { m_newConnectionCallback = cb; }

    bool listenning() const { return m_listenning; }
    void listen();

private:
    void handleRead();

    EventLoop *m_eventLoop;
    std::unique_ptr<Socket> m_acceptSocketPtr;
    std::unique_ptr<Channel> m_acceptChannelPtr;
    NewConnectionCallback m_newConnectionCallback;
    bool m_listenning;
    int m_idleFd;
};

} // namespace net

} // namespace youth

#endif // ACCEPTOR_H
