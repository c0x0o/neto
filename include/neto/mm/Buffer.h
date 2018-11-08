#ifndef __NETO_MM_BUFFER_H__
#define __NETO_MM_BUFFER_H__

#include <string.h>

#include <memory>
#include <vector>

#include <neto/mm/endian.h>
#include <neto/mm/MemoryBlock.h>

namespace neto {
namespace mm {
  class Buffer;

  typedef uint8_t byte;
  typedef std::shared_ptr<Buffer> BufferPtr;
  typedef std::weak_ptr<Buffer> BufferWeakPtr;

  // optimized with copy-on-write
  // a Buffer is expected to be used in only one thread
  class Buffer {
    public:
      static BufferPtr alloc(size_t size);
      static BufferPtr concat(const std::vector<Buffer *> &vec);

      Buffer(size_t size = 0);
      Buffer(const MemoryBlockPtr &);
      Buffer(const Buffer &);
      Buffer(const Buffer &&);

      size_t length() const {return end_-start_;}
      // users are not expected to call 'size'
      size_t size() const {return data_->size();}
      // do not call 'free' or 'delete' on returned pointer
      byte *data() const {return data_->begin() + start_;}
      byte *memory() const {return data_->begin();}
      // len = 0 will return a Buffer with content [start, end_]
      BufferPtr slice(size_t start, size_t len = 0) const;

      // no bounder check, user has resiponsibility to do this
      void write(size_t pos, const byte *, size_t len);
      void writeByte(size_t pos, byte);
      void writeUInt16LE(size_t pos, uint16_t data);
      void writeUInt32LE(size_t pos, uint32_t data);
      void writeUInt64LE(size_t pos, uint64_t data);
      void writeUInt16BE(size_t pos, uint16_t data);
      void writeUInt32BE(size_t pos, uint32_t data);
      void writeUInt64BE(size_t pos, uint64_t data);

      // size of returned buffer may smaller than len
      BufferPtr read(size_t pos, size_t len) const;
      byte readByte(size_t pos) const;
      uint16_t readUInt16LE(size_t pos) const;
      uint32_t readUInt32LE(size_t pos) const;
      uint64_t readUInt64LE(size_t pos) const;
      uint16_t readUInt16BE(size_t pos) const;
      uint32_t readUInt32BE(size_t pos) const;
      uint64_t readUInt64BE(size_t pos) const;
    private:
      MemoryBlockPtr data_;
      size_t start_;
      size_t end_;
  };

} // namespace mm
} // namespace neto

#endif // __NETO_MM_BUFFER_H__
