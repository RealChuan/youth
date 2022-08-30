#pragma once

#include <youth/core/DateTime.hpp>
#include <youth/core/Object.h>
#include <youth/net/Callbacks.h>

namespace google {
namespace protobuf {
class Message;
}
} // namespace google

namespace youth {

namespace rpc {

using MessagePtr = std::shared_ptr<google::protobuf::Message>;

class ProtobufCodecLite : core::noncopyable
{
public:
    const static int kHeaderLen = sizeof(int32_t);
    const static int kChecksumLen = sizeof(int32_t);
    const static int kMaxMessageLen = 64 * 1024
                                      * 1024; // same as m_codecstream.h kDefaultTotalBytesLimit

    enum ErrorCode {
        kNoError = 0,
        kInvalidLength,
        kCheckSumError,
        kInvalidNameLen,
        kUnknownMessageType,
        kParseError
    };

    // return false to stop parsing protobuf message
    using RawMessageCallback
        = std::function<bool(const net::TcpConnectionPtr &, std::string_view, core::DateTime)>;

    using ProtobufMessageCallback
        = std::function<void(const net::TcpConnectionPtr &, const MessagePtr &, DateTime)>;

    using ErrorCallback
        = std::function<void(const net::TcpConnectionPtr &, net::Buffer *, DateTime, ErrorCode)>;

    ProtobufCodecLite(const ::google::protobuf::Message *prototype,
                      std::string_view tagArg,
                      const ProtobufMessageCallback &messageCb,
                      const RawMessageCallback &rawCb = RawMessageCallback(),
                      const ErrorCallback &errorCb = defaultErrorCallback)
        : m_prototype(prototype)
        , m_tag(tagArg)
        , m_messageCallback(messageCb)
        , m_rawCb(rawCb)
        , m_errorCallback(errorCb)
        , kMinMessageLen(static_cast<int>(tagArg.size() + kChecksumLen))
    {}

    virtual ~ProtobufCodecLite() = default;

    const std::string &tag() const { return m_tag; }

    void send(const net::TcpConnectionPtr &conn, const ::google::protobuf::Message &message);

    void onMessage(const net::TcpConnectionPtr &conn, net::Buffer *buf, DateTime receiveTime);

    virtual bool parseFromBuffer(std::string_view buf, google::protobuf::Message *message);
    virtual int serializeToBuffer(const google::protobuf::Message &message, net::Buffer *buf);

    static const std::string &errorCodeToString(ErrorCode errorCode);

    // public for unit tests
    ErrorCode parse(const char *buf, int len, ::google::protobuf::Message *message);
    void fillEmptyBuffer(net::Buffer *buf, const google::protobuf::Message &message);

    static int32_t checksum(const void *buf, int len);
    static bool validateChecksum(const char *buf, int len);
    static int32_t asInt32(const char *buf);
    static void defaultErrorCallback(const net::TcpConnectionPtr &,
                                     net::Buffer *,
                                     DateTime,
                                     ErrorCode);

private:
    const ::google::protobuf::Message *m_prototype;
    const std::string m_tag;
    ProtobufMessageCallback m_messageCallback;
    RawMessageCallback m_rawCb;
    ErrorCallback m_errorCallback;
    const int kMinMessageLen;
};

template<typename MSG,
         const char *TAG,
         typename CODEC = ProtobufCodecLite> // TAG must be a variable with external
                                             // linkage, not a string literal
class ProtobufCodecLiteT
{
    static_assert(std::is_base_of<ProtobufCodecLite, CODEC>::value,
                  "CODEC should be derived from ProtobufCodecLite");

public:
    typedef std::shared_ptr<MSG> ConcreteMessagePtr;
    typedef std::function<void(const net::TcpConnectionPtr &, const ConcreteMessagePtr &, DateTime)>
        ProtobufMessageCallback;
    typedef ProtobufCodecLite::RawMessageCallback RawMessageCallback;
    typedef ProtobufCodecLite::ErrorCallback ErrorCallback;

    explicit ProtobufCodecLiteT(
        const ProtobufMessageCallback &messageCb,
        const RawMessageCallback &rawCb = RawMessageCallback(),
        const ErrorCallback &errorCb = ProtobufCodecLite::defaultErrorCallback)
        : m_messageCallback(messageCb)
        , m_codec(&MSG::default_instance(),
                  TAG,
                  std::bind(&ProtobufCodecLiteT::onRpcMessage,
                            this,
                            std::placeholders::_1,
                            std::placeholders::_2,
                            std::placeholders::_3),
                  rawCb,
                  errorCb)
    {}

    const std::string &tag() const { return m_codec.tag(); }

    void send(const net::TcpConnectionPtr &conn, const MSG &message)
    {
        m_codec.send(conn, message);
    }

    void onMessage(const net::TcpConnectionPtr &conn, net::Buffer *buf, DateTime receiveTime)
    {
        m_codec.onMessage(conn, buf, receiveTime);
    }

    // internal
    void onRpcMessage(const net::TcpConnectionPtr &conn,
                      const MessagePtr &message,
                      DateTime receiveTime)
    {
        m_messageCallback(conn, std::static_pointer_cast<MSG>(message), receiveTime);
    }

    void fillEmptyBuffer(net::Buffer *buf, const MSG &message)
    {
        m_codec.fillEmptyBuffer(buf, message);
    }

private:
    ProtobufMessageCallback m_messageCallback;
    CODEC m_codec;
};

} // namespace rpc

} // namespace youth
