#include <neto/mm/Buffer.h>

#include <gtest/gtest.h>

#include <stdio.h>
#include <string.h>
#include <endian.h>

using namespace neto::mm;

TEST(Buffer, constructor) {
  Buffer a(10), b(a), c(Buffer(10));

  EXPECT_EQ(a.size(), 10);
  EXPECT_EQ(a.length(), 10);
  EXPECT_EQ(b.size(), 10);
  EXPECT_EQ(b.length(), 10);
  EXPECT_EQ(c.size(), 10);
  EXPECT_EQ(c.length(), 10);
  EXPECT_EQ(a.data(), a.memory());
}

TEST(Buffer, alloc) {
  BufferPtr ptr = Buffer::alloc(10);

  EXPECT_EQ(ptr->size(), 10);
  EXPECT_EQ(ptr->length(), 10);
}

TEST(Buffer, concat) {
  Buffer a(10), b(10), c(10);

  memcpy(a.data(), "123456789", 10);
  memcpy(b.data(), "123456789", 10);
  memcpy(c.data(), "123456789", 10);

  BufferPtr ptr = Buffer::concat(std::vector<Buffer *>({&a,&b,&c}));

  EXPECT_EQ(ptr->size(), 30);
  EXPECT_EQ(ptr->length(), 30);
  EXPECT_EQ(memcmp(ptr->data(), "123456789", 10), 0);
  EXPECT_EQ(memcmp(ptr->data() + 10, "123456789", 10), 0);
  EXPECT_EQ(memcmp(ptr->data() + 20, "123456789", 10), 0);
}

TEST(Buffer, slice) {
  Buffer a(10);

  memcpy(a.data(), "123456789", 10);

  BufferPtr slice = a.slice(0, 5);

  EXPECT_EQ(slice->size(), 10);
  EXPECT_EQ(slice->length(), 5);
  EXPECT_EQ(slice->data(), a.data());
  EXPECT_EQ(slice->memory(), a.memory());
  EXPECT_EQ(memcmp(slice->data(), "1234567890", 5), 0);
}

TEST(Buffer, read) {
  uint16_t portBE = htobe16(52427);
  Buffer a(10);

  memcpy(a.data(), &portBE, 2);

  EXPECT_EQ(*(uint16_t *)a.read(0, 2)->data(), portBE);
  // read the number(host endian) as LE
  EXPECT_EQ(a.readUInt16LE(0), portBE);
  // read the number(host endian) as BE
  EXPECT_EQ(a.readUInt16BE(0), 52427);
}

TEST(Buffer, write) {
  uint16_t port = 52427;
  Buffer a(10);

  // write the number(host endian) as BE
  a.writeUInt16BE(0, port);

  EXPECT_EQ(a.readUInt16LE(0), htobe16(52427));
  EXPECT_EQ(a.readUInt16BE(0), 52427);
}
