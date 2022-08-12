#pragma once

#include <google/protobuf/io/zero_copy_stream.h>

#include <youth/net/Buffer.h>

namespace youth {

namespace rpc {

class BufferOutputStream : public google::protobuf::io::ZeroCopyOutputStream {
public:
  BufferOutputStream(Buffer *buf)
      : m_buffer(CHECK_NOTNULL(buf)),
        m_originalSize(m_buffer->readableBytes()) {}

  virtual bool next(void **data, int *size) // override
  {
    m_buffer->ensureWritableBytes(4096);
    *data = m_buffer->beginWrite();
    *size = static_cast<int>(m_buffer->writableBytes());
    m_buffer->hasWritten(*size);
    return true;
  }

  virtual void backUp(int count) // override
  {
    m_buffer->unwrite(count);
  }

  virtual int64_t byteCount() const // override
  {
    return m_buffer->readableBytes() - m_originalSize;
  }

private:
  Buffer *m_buffer;
  size_t m_originalSize;
};

} // namespace rpc

} // namespace youth