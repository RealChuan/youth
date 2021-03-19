#ifndef BUFFER_H
#define BUFFER_H

#include <string_view>
#include <vector>
#include <string>

#include <youth/core/Object.h>

namespace youth
{

using namespace core;

namespace net
{

class Buffer : copyable
{
public:
    static const size_t kCheapPrepend = 8;
    static const size_t kInitialSize = 1024;

    explicit Buffer(size_t initialSize = kInitialSize);

    // implicit copy-ctor, move-ctor, dtor and assignment are fine
    // NOTE: implicit move-ctor is added in g++ 4.6

    void swap(Buffer& rhs);

    size_t readableBytes() const
    { return m_writerIndex - m_readerIndex; }
    size_t writableBytes() const
    { return m_bufferVec.size() - m_writerIndex; }
    size_t prependableBytes() const
    { return m_readerIndex; }

    const char* peek() const
    { return begin() + m_readerIndex; }

    const char* findCRLF() const;
    const char* findCRLF(const char* start) const;
    const char* findEOL() const;
    const char* findEOL(const char* start) const;

    // retrieve returns void, to prevent
    // string str(retrieve(readableBytes()), readableBytes());
    // the evaluation of two functions are unspecified
    void retrieve(size_t len);
    void retrieveUntil(const char* end);
    void retrieveInt64() { retrieve(sizeof(int64_t)); }
    void retrieveInt32() { retrieve(sizeof(int32_t)); }
    void retrieveInt16() { retrieve(sizeof(int16_t)); }
    void retrieveInt8() { retrieve(sizeof(int8_t)); }
    void retrieveAll()
    { m_readerIndex = kCheapPrepend; m_writerIndex = kCheapPrepend; }
    std::string retrieveAllAsString()
    { return retrieveAsString(readableBytes()); }
    std::string retrieveAsString(size_t len);
    std::string_view toStringPiece() const
    { return std::string_view(peek(), int(readableBytes())); }

    void append(const std::string_view& str)
    { append(str.data(), str.size()); }
    void append(const char* /*restrict*/ data, size_t len);
    void append(const void* /*restrict*/ data, size_t len)
    { append(static_cast<const char*>(data), len); }

    void ensureWritableBytes(size_t len);

    char* beginWrite() { return begin() + m_writerIndex; }
    const char* beginWrite() const { return begin() + m_writerIndex; }
    void hasWritten(size_t len);
    void unwrite(size_t len);

    ///
    /// Append int64_t using network endian
    ///
    ///
    void appendInt64(int64_t x);
    ///
    /// Append int32_t using network endian
    ///
    void appendInt32(int32_t x);
    void appendInt16(int16_t x);
    void appendInt8(int8_t x) { append(&x, sizeof x); }

    ///
    /// Read int64_t from network endian
    ///
    /// Require: buf->readableBytes() >= sizeof(int32_t)
    int64_t readInt64();

    ///
    /// Read int32_t from network endian
    ///
    /// Require: buf->readableBytes() >= sizeof(int32_t)
    int32_t readInt32();
    int16_t readInt16();
    int8_t readInt8();

    ///
    /// Peek int64_t from network endian
    ///
    /// Require: buf->readableBytes() >= sizeof(int64_t)
    int64_t peekInt64() const;

    ///
    /// Peek int32_t from network endian
    ///
    /// Require: buf->readableBytes() >= sizeof(int32_t)
    int32_t peekInt32() const;
    int16_t peekInt16() const;
    int8_t peekInt8() const;

    ///
    /// Prepend int64_t using network endian
    ///
    void prependInt64(int64_t x);

    ///
    /// Prepend int32_t using network endian
    ///
    void prependInt32(int32_t x);
    void prependInt16(int16_t x);
    void prependInt8(int8_t x) { prepend(&x, sizeof x); }
    void prepend(const void* /*restrict*/ data, size_t len);

    void shrink(size_t reserve);

    size_t internalCapacity() const { return m_bufferVec.capacity(); }

    /// Read data directly into buffer.
    ///
    /// It may implement with readv(2)
    /// @return result of read(2), @c errno is saved
    ssize_t readFd(int fd, int* savedErrno);

private:
    char* begin() { return &*m_bufferVec.begin(); }
    const char* begin() const { return &*m_bufferVec.begin(); }

    void makeSpace(size_t len);

    std::vector<char> m_bufferVec;
    size_t m_readerIndex;
    size_t m_writerIndex;

    static const char kCRLF[];
};

}

}

#endif // BUFFER_H
