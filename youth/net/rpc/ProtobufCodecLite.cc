#include "ProtobufCodecLite.hpp"

#include <youth/net/Buffer.h>
#include <youth/net/SocketFunc.h>
#include <youth/net/TcpConnection.h>
#include <youth/utils/Logging.h>

#include <google/protobuf/message.h>
#include <zlib.h>

#include <assert.h>

namespace youth {

using namespace utils;

namespace rpc {

void ProtobufCodecLite::send(const net::TcpConnectionPtr &conn,
                             const ::google::protobuf::Message &message)
{
    // FIXME: serialize to TcpConnection::outputBuffer()
    net::Buffer buf;
    fillEmptyBuffer(&buf, message);
    conn->send(&buf);
}

void ProtobufCodecLite::fillEmptyBuffer(net::Buffer *buf, const google::protobuf::Message &message)
{
    assert(buf->readableBytes() == 0);
    // FIXME: can we move serialization & checksum to other thread?
    buf->append(m_tag);

    auto byte_size = serializeToBuffer(message, buf);

    auto checkSum = checksum(buf->peek(), static_cast<int>(buf->readableBytes()));
    buf->appendInt32(checkSum);
    assert(buf->readableBytes() == m_tag.size() + byte_size + kChecksumLen);
    (void) byte_size;
    auto len = net::SocketFunc::hostToNetwork32(static_cast<int32_t>(buf->readableBytes()));
    buf->prepend(&len, sizeof len);
}

void ProtobufCodecLite::onMessage(const net::TcpConnectionPtr &conn,
                                  net::Buffer *buf,
                                  DateTime receiveTime)
{
    while (buf->readableBytes() >= static_cast<uint32_t>(kMinMessageLen + kHeaderLen)) {
        const auto len = buf->peekInt32();
        if (len > kMaxMessageLen || len < kMinMessageLen) {
            m_errorCallback(conn, buf, receiveTime, kInvalidLength);
            break;
        } else if (buf->readableBytes() >= static_cast<size_t>(kHeaderLen + len)) {
            if (m_rawCb
                && !m_rawCb(conn, std::string_view(buf->peek(), kHeaderLen + len), receiveTime)) {
                buf->retrieve(kHeaderLen + len);
                continue;
            }
            MessagePtr message(m_prototype->New());
            // FIXME: can we move deserialization & callback to other thread?
            auto errorCode = parse(buf->peek() + kHeaderLen, len, message.get());
            if (errorCode == kNoError) {
                // FIXME: try { } catch (...) { }
                m_messageCallback(conn, message, receiveTime);
                buf->retrieve(kHeaderLen + len);
            } else {
                m_errorCallback(conn, buf, receiveTime, errorCode);
                break;
            }
        } else {
            break;
        }
    }
}

bool ProtobufCodecLite::parseFromBuffer(std::string_view buf, google::protobuf::Message *message)
{
    return message->ParseFromArray(buf.data(), static_cast<int>(buf.size()));
}

int ProtobufCodecLite::serializeToBuffer(const google::protobuf::Message &message, net::Buffer *buf)
{
    auto byte_size = google::protobuf::internal::ToIntSize(message.ByteSizeLong());
    buf->ensureWritableBytes(byte_size + kChecksumLen);

    auto start = reinterpret_cast<uint8_t *>(buf->beginWrite());
    auto end = message.SerializeWithCachedSizesToArray(start);
    if (end - start != byte_size) {
        LOG_WARN << "end - start != byte_siz";
    }
    buf->hasWritten(byte_size);
    return byte_size;
}

namespace {
const std::string kNoErrorStr = "NoError";
const std::string kInvalidLengthStr = "InvalidLength";
const std::string kCheckSumErrorStr = "CheckSumError";
const std::string kInvalidNameLenStr = "InvalidNameLen";
const std::string kUnknownMessageTypeStr = "UnknownMessageType";
const std::string kParseErrorStr = "ParseError";
const std::string kUnknownErrorStr = "UnknownError";
} // namespace

const std::string &ProtobufCodecLite::errorCodeToString(ErrorCode errorCode)
{
    switch (errorCode) {
    case kNoError: return kNoErrorStr;
    case kInvalidLength: return kInvalidLengthStr;
    case kCheckSumError: return kCheckSumErrorStr;
    case kInvalidNameLen: return kInvalidNameLenStr;
    case kUnknownMessageType: return kUnknownMessageTypeStr;
    case kParseError: return kParseErrorStr;
    default: return kUnknownErrorStr;
    }
}

void ProtobufCodecLite::defaultErrorCallback(const net::TcpConnectionPtr &conn,
                                             net::Buffer *buf,
                                             DateTime,
                                             ErrorCode errorCode)
{
    LOG_ERROR << "ProtobufCodecLite::defaultErrorCallback - " << errorCodeToString(errorCode);
    if (conn && conn->connected()) {
        conn->shutdown();
    }
}

int32_t ProtobufCodecLite::asInt32(const char *buf)
{
    int32_t be32 = 0;
    ::memcpy(&be32, buf, sizeof(be32));
    return net::SocketFunc::networkToHost32(be32);
}

int32_t ProtobufCodecLite::checksum(const void *buf, int len)
{
    return static_cast<int32_t>(::adler32(1, static_cast<const Bytef *>(buf), len));
}

bool ProtobufCodecLite::validateChecksum(const char *buf, int len)
{
    // check sum
    auto expectedCheckSum = asInt32(buf + len - kChecksumLen);
    auto checkSum = checksum(buf, len - kChecksumLen);
    return checkSum == expectedCheckSum;
}

ProtobufCodecLite::ErrorCode ProtobufCodecLite::parse(const char *buf,
                                                      int len,
                                                      ::google::protobuf::Message *message)
{
    auto error = kNoError;

    if (validateChecksum(buf, len)) {
        if (memcmp(buf, m_tag.data(), m_tag.size()) == 0) {
            // parse from buffer
            auto data = buf + m_tag.size();
            auto dataLen = len - kChecksumLen - static_cast<int>(m_tag.size());
            if (parseFromBuffer(std::string_view(data, dataLen), message)) {
                error = kNoError;
            } else {
                error = kParseError;
            }
        } else {
            error = kUnknownMessageType;
        }
    } else {
        error = kCheckSumError;
    }

    return error;
}

} // namespace rpc

} // namespace youth
