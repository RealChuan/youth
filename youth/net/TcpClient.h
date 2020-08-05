#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <memory>

#include <youth/core/Object.h>

namespace youth
{

class Connector;
typedef std::shared_ptr<Connector> ConnectorPtr;

class EventLoop;
class TcpAddressInfo;
class TcpClient : noncopyable
{
public:
    TcpClient(EventLoop* loop,
              const TcpAddressInfo& serverAddr,
              const std::string& nameArg);
    ~TcpClient();

    void connect();
    void disconnect();
    void stop();

};

}

#endif // TCPCLIENT_H
