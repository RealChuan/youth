#include "Buffer.h"
#include "SocketFunc.h"

#include <assert.h>
#include <algorithm>
#include <memory>
#include <string.h>
#include <string_view>

namespace youth
{

namespace net
{

Buffer::Buffer(size_t initialSize)
    : m_bufferVec(kCheapPrepend + initialSize)
    , m_readerIndex(kCheapPrepend)
    , m_writerIndex(kCheapPrepend)
{
    assert(readableBytes() == 0);
    assert(writableBytes() == initialSize);
    assert(prependableBytes() == kCheapPrepend);
}

void Buffer::swap(Buffer &rhs)
{
    m_bufferVec.swap(rhs.m_bufferVec);
    std::swap(m_readerIndex, rhs.m_readerIndex);
    std::swap(m_writerIndex, rhs.m_writerIndex);
}

size_t Buffer::readableBytes() const
{
    return m_writerIndex - m_readerIndex;
}

size_t Buffer::writableBytes() const
{
    return m_bufferVec.size() - m_writerIndex;
}

size_t Buffer::prependableBytes() const
{
    return m_readerIndex;
}

const char *Buffer::peek() const
{
    return begin() + m_readerIndex;
}

const char *Buffer::findCRLF() const
{
    // FIXME: replace with memmem()?
    const char* crlf = std::search(peek(), beginWrite(), kCRLF, kCRLF+2);
    return crlf == beginWrite() ? NULL : crlf;
}

const char *Buffer::findCRLF(const char *start) const
{
    assert(peek() <= start);
    assert(start <= beginWrite());
    // FIXME: replace with memmem()?
    const char* crlf = std::search(start, beginWrite(), kCRLF, kCRLF+2);
    return crlf == beginWrite() ? NULL : crlf;
}

const char *Buffer::findEOL() const
{
    const void* eol = memchr(peek(), '\n', readableBytes());
    return static_cast<const char*>(eol);
}

const char *Buffer::findEOL(const char *start) const
{
    assert(peek() <= start);
    assert(start <= beginWrite());
    const void* eol = memchr(start, '\n', beginWrite() - start);
    return static_cast<const char*>(eol);
}

void Buffer::retrieve(size_t len)
{
    assert(len <= readableBytes());
    if (len < readableBytes())
    {
        m_readerIndex += len;
    }
    else
    {
        retrieveAll();
    }
}

void Buffer::retrieveUntil(const char *end)
{
    assert(peek() <= end);
    assert(end <= beginWrite());
    retrieve(end - peek());
}

void Buffer::retrieveInt64()
{
    retrieve(sizeof(int64_t));
}

void Buffer::retrieveInt32()
{
    retrieve(sizeof(int32_t));
}

void Buffer::retrieveInt16()
{
    retrieve(sizeof(int16_t));
}

void Buffer::retrieveInt8()
{
    retrieve(sizeof(int8_t));
}

void Buffer::retrieveAll()
{
    m_readerIndex = kCheapPrepend;
    m_writerIndex = kCheapPrepend;
}

std::string Buffer::retrieveAllAsString()
{
    return retrieveAsString(readableBytes());
}

std::string Buffer::retrieveAsString(size_t len)
{
    assert(len <= readableBytes());
    std::string result(peek(), len);
    retrieve(len);
    return result;
}

std::string_view Buffer::toStringPiece() const
{
    return std::string_view(peek(), static_cast<int>(readableBytes()));
}

void Buffer::append(const std::string_view &str)
{
    append(str.data(), str.size());
}

void Buffer::append(const char *data, size_t len)
{
    ensureWritableBytes(len);
    std::copy(data, data+len, beginWrite());
    hasWritten(len);
}

void Buffer::append(const void *data, size_t len)
{
    append(static_cast<const char*>(data), len);
}

void Buffer::ensureWritableBytes(size_t len)
{
    if (writableBytes() < len)
    {
        makeSpace(len);
    }
    assert(writableBytes() >= len);
}

char *Buffer::beginWrite()
{ return begin() + m_writerIndex; }

const char *Buffer::beginWrite() const
{ return begin() + m_writerIndex; }

void Buffer::hasWritten(size_t len)
{
    assert(len <= writableBytes());
    m_writerIndex += len;
}

void Buffer::unwrite(size_t len)
{
    assert(len <= readableBytes());
    m_writerIndex -= len;
}

void Buffer::appendInt64(int64_t x)
{
    int64_t be64 = SocketFunc::hostToNetwork64(x);
    append(&be64, sizeof be64);
}

void Buffer::appendInt32(int32_t x)
{
    int32_t be32 = SocketFunc::hostToNetwork32(x);
    append(&be32, sizeof be32);
}

void Buffer::appendInt16(int16_t x)
{
    int16_t be16 = SocketFunc::hostToNetwork16(x);
    append(&be16, sizeof be16);
}

void Buffer::appendInt8(int8_t x)
{
    append(&x, sizeof x);
}

int64_t Buffer::readInt64()
{
    int64_t result = peekInt64();
    retrieveInt64();
    return result;
}

int32_t Buffer::readInt32()
{
    int32_t result = peekInt32();
    retrieveInt32();
    return result;
}

int16_t Buffer::readInt16()
{
    int16_t result = peekInt16();
    retrieveInt16();
    return result;
}

int8_t Buffer::readInt8()
{
    int8_t result = peekInt8();
    retrieveInt8();
    return result;
}

int64_t Buffer::peekInt64() const
{
    assert(readableBytes() >= sizeof(int64_t));
    int64_t be64 = 0;
    ::memcpy(&be64, peek(), sizeof be64);
    return SocketFunc::networkToHost64(be64);
}

int32_t Buffer::peekInt32() const
{
    assert(readableBytes() >= sizeof(int32_t));
    int32_t be32 = 0;
    ::memcpy(&be32, peek(), sizeof be32);
    return SocketFunc::networkToHost32(be32);
}

int16_t Buffer::peekInt16() const
{
    assert(readableBytes() >= sizeof(int16_t));
    int16_t be16 = 0;
    ::memcpy(&be16, peek(), sizeof be16);
    return SocketFunc::networkToHost16(be16);
}

int8_t Buffer::peekInt8() const
{
    assert(readableBytes() >= sizeof(int8_t));
    int8_t x = *peek();
    return x;
}

void Buffer::prependInt64(int64_t x)
{
    int64_t be64 = SocketFunc::hostToNetwork64(x);
    prepend(&be64, sizeof be64);
}

void Buffer::prependInt32(int32_t x)
{
    int32_t be32 = SocketFunc::hostToNetwork32(x);
    prepend(&be32, sizeof be32);
}

void Buffer::prependInt16(int16_t x)
{
    int16_t be16 = SocketFunc::hostToNetwork16(x);
    prepend(&be16, sizeof be16);
}

void Buffer::prependInt8(int8_t x)
{
    prepend(&x, sizeof x);
}

void Buffer::prepend(const void *data, size_t len)
{
    assert(len <= prependableBytes());
    m_readerIndex -= len;
    const char* d = static_cast<const char*>(data);
    std::copy(d, d+len, begin()+m_readerIndex);
}

void Buffer::shrink(size_t reserve)
{
    // FIXME: use vector::shrink_to_fit() in C++ 11 if possible.
    Buffer other;
    other.ensureWritableBytes(readableBytes()+reserve);
    other.append(toStringPiece());
    swap(other);
}

size_t Buffer::internalCapacity() const
{
    return m_bufferVec.capacity();
}

ssize_t Buffer::readFd(int fd, int *savedErrno)
{
    // saved an ioctl()/FIONREAD call to tell how much to read
    char extrabuf[65536];
    struct iovec vec[2];
    const size_t writable = writableBytes();
    vec[0].iov_base = begin()+m_writerIndex;
    vec[0].iov_len = writable;
    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof extrabuf;
    // when there is enough space in this buffer, don't read into extrabuf.
    // when extrabuf is used, we read 128k-1 bytes at most.
    const int iovcnt = (writable < sizeof extrabuf) ? 2 : 1;
    const ssize_t n = SocketFunc::readv(fd, vec, iovcnt);
    if (n < 0)
    {
        *savedErrno = errno;
    }
    else if (static_cast<size_t>(n) <= writable)
    {
        m_writerIndex += n;
    }
    else
    {
        m_writerIndex = m_bufferVec.size();
        append(extrabuf, n - writable);
    }
    // if (n == writable + sizeof extrabuf)
    // {
    //   goto line_30;
    // }
    return n;
}

char *Buffer::begin()
{
    return &*m_bufferVec.begin();
}

const char *Buffer::begin() const
{
    return &*m_bufferVec.begin();
}

void Buffer::makeSpace(size_t len)
{
    if (writableBytes() + prependableBytes() < len + kCheapPrepend)
    {
        // FIXME: move readable data
        m_bufferVec.resize(m_writerIndex + len);
    }
    else
    {
        // move readable data to the front, make space inside buffer
        assert(kCheapPrepend < m_readerIndex);
        size_t readable = readableBytes();
        std::copy(begin() + m_readerIndex,
                  begin() + m_writerIndex,
                  begin() + kCheapPrepend);
        m_readerIndex = kCheapPrepend;
        m_writerIndex = m_readerIndex + readable;
        assert(readable == readableBytes());
    }
}

}

}
