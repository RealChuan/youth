#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <functional>
#include <memory>

#include <youth/core/DateTime.hpp>

namespace youth {

using namespace core;

namespace net {

class Buffer;
class TcpConnection;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using TimerCallback = std::function<void()>;
using ConnectionCallback = std::function<void(const TcpConnectionPtr &)>;
using CloseCallback = std::function<void(const TcpConnectionPtr &)>;
using WriteCompleteCallback = std::function<void(const TcpConnectionPtr &)>;
using HighWaterMarkCallback = std::function<void(const TcpConnectionPtr &, size_t)>;

// the data has been read to (buf, len)
using MessageCallback = std::function<void(const TcpConnectionPtr &, Buffer *, DateTime)>;

void defaultConnectionCallback(const TcpConnectionPtr &conn);
void defaultMessageCallback(const TcpConnectionPtr &conn, Buffer *buffer, DateTime receiveTime);

} // namespace net

} // namespace youth

#endif // CALLBACKS_H
