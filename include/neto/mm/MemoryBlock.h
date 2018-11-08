#ifndef __NETO_MM_MEMORYBLOCK_H__
#define __NETO_MM_MEMORYBLOCK_H__

#include <string.h>

#include <memory>

namespace neto {
namespace mm {
  class MemoryBlock;

  typedef std::shared_ptr<uint8_t> BytesPtr;
  typedef std::shared_ptr<MemoryBlock> MemoryBlockPtr;
  typedef std::weak_ptr<MemoryBlock> MemoryBlockWeakPtr;

  class MemoryBlock {
    public:
      typedef uint8_t * iterator;

      static MemoryBlockPtr alloc(size_t size);

      MemoryBlock(size_t size);
      MemoryBlock(size_t size, const BytesPtr &ptr);
      ~MemoryBlock();

      size_t size() const {return size_;}
      iterator begin() const {return data_.get();}
      iterator end() const {return data_.get() + size_;}
      bool shared() const {return data_.use_count() > 1;}

      MemoryBlockPtr share();
      MemoryBlockPtr copy();
      int split();
    private:
      BytesPtr data_;
      size_t size_;
  };

} // namespace mm
} // namespace neto

#endif // __NETO_MM_MEMORYBLOCK_H__
