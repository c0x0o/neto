#include <neto/mm/Buffer.h>

using namespace neto::mm;
using std::make_shared;
using std::vector;

Buffer::Buffer(size_t size)
  : data_(MemoryBlock::alloc(size)),
    start_(0),
    end_(size)
{
}

Buffer::Buffer(const MemoryBlockPtr & mb)
  : data_(mb),
    start_(0),
    end_(0)
{
}

Buffer::Buffer(const Buffer &buff)
  : data_(buff.data_),
    start_(buff.start_),
    end_(buff.end_)
{
}

Buffer::Buffer(const Buffer &&buff)
  : data_(buff.data_),
    start_(buff.start_),
    end_(buff.end_)
{
}

BufferPtr Buffer::alloc(size_t size) {
  return make_shared<Buffer>(size);
}

// NOTE: Need nicer implementation
BufferPtr Buffer::concat(const vector<Buffer *> &vec) {
  size_t len = 0;

  for (auto iter = vec.begin(); iter != vec.end(); iter++) {
    len += (*iter)->length();
  }

  BufferPtr ptr = make_shared<Buffer>(len);
  size_t pos = 0;

  for (auto iter = vec.begin(); iter != vec.end(); iter++) {
    memcpy(ptr->data() + pos, (*iter)->data(), (*iter)->length());
    pos += (*iter)->length();
  }

  return ptr;
}

BufferPtr Buffer::slice(size_t pos, size_t len) const {
  BufferPtr ptr = make_shared<Buffer>(data_);

  ptr->start_ = start_ + pos;
  ptr->end_ = start_ + len;

  return ptr;
}

BufferPtr Buffer::read(size_t pos, size_t len) const {
  BufferPtr ptr = make_shared<Buffer>(*this);
  size_t end;

  if (pos > length()) {
    ptr->start_ = start_;
    ptr->end_ = start_;
  } else {
    if (pos + len > length()) {
      end = end_;
    } else {
      end = start_ + pos + len;
    }

    ptr->start_ = start_ + pos;
    ptr->end_ = end;
  }

  return ptr;
}

byte Buffer::readByte(size_t pos) const {
  return *(data() + pos);
}

uint16_t Buffer::readUInt16LE(size_t pos) const {
  return hostToLE(*((uint16_t *)(data() + pos)));
}

uint32_t Buffer::readUInt32LE(size_t pos) const {
  return hostToLE(*((uint32_t *)(data() + pos)));
}

uint64_t Buffer::readUInt64LE(size_t pos) const {
  return hostToLE(*((uint64_t *)(data() + pos)));
}

uint16_t Buffer::readUInt16BE(size_t pos) const {
  return hostToBE(*((uint16_t *)(data() + pos)));
}

uint32_t Buffer::readUInt32BE(size_t pos) const {
  return hostToBE(*((uint32_t *)(data() + pos)));
}

uint64_t Buffer::readUInt64BE(size_t pos) const {
  return hostToBE(*((uint64_t *)(data() + pos)));
}

void Buffer::write(size_t pos, const byte *bytes, size_t len) {
  if (data_->shared()) {
    data_->split();
  }

  memcpy(data() + pos, bytes, len);
}

void Buffer::writeByte(size_t pos, byte b) {
  if (data_->shared()) {
    data_->split();
  }

  byte *where = data() + pos;
  *where = b;
}

void Buffer::writeUInt16LE(size_t pos, uint16_t d) {
  if (data_->shared()) {
    data_->split();
  }

  uint16_t *where = (uint16_t *)(data() + pos);
  *where = hostToLE(d);
}

void Buffer::writeUInt32LE(size_t pos, uint32_t d) {
  if (data_->shared()) {
    data_->split();
  }

  uint32_t *where = (uint32_t *)(data() + pos);
  *where = hostToLE(d);
}

void Buffer::writeUInt64LE(size_t pos, uint64_t d) {
  if (data_->shared()) {
    data_->split();
  }

  uint64_t *where = (uint64_t *)(data() + pos);
  *where = hostToLE(d);
}

void Buffer::writeUInt16BE(size_t pos, uint16_t d) {
  if (data_->shared()) {
    data_->split();
  }

  uint16_t *where = (uint16_t *)(data() + pos);
  *where = hostToBE(d);
}

void Buffer::writeUInt32BE(size_t pos, uint32_t d) {
  if (data_->shared()) {
    data_->split();
  }

  uint32_t *where = (uint32_t *)(data() + pos);
  *where = hostToBE(d);
}

void Buffer::writeUInt64BE(size_t pos, uint64_t d) {
  if (data_->shared()) {
    data_->split();
  }

  uint64_t *where = (uint64_t *)(data() + pos);
  *where = hostToBE(d);
}

