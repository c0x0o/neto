#include <neto/mm/MemoryBlock.h>

using namespace neto::mm;
using std::make_shared;

MemoryBlock::MemoryBlock(size_t size)
  : data_(new uint8_t[size], std::default_delete<uint8_t[]>()),
    size_(size)
{
}
MemoryBlock::MemoryBlock(size_t size, const BytesPtr &ptr)
  : data_(ptr),
    size_(size)
{
}

MemoryBlock::~MemoryBlock() {}

MemoryBlockPtr MemoryBlock::alloc(size_t size) {
  return make_shared<MemoryBlock>(size);
}

MemoryBlockPtr MemoryBlock::share() {
  return make_shared<MemoryBlock>(size_, data_);
}

MemoryBlockPtr MemoryBlock::copy() {
  MemoryBlockPtr ptr(make_shared<MemoryBlock>(size_));

  memcpy(ptr->begin(), begin(), size_);

  return ptr;
}

int MemoryBlock::split() {
  BytesPtr ptr(new uint8_t[size_], std::default_delete<uint8_t[]>());

  memcpy(ptr.get(), begin(), size_);

  data_ = ptr;

  return 0;
}

