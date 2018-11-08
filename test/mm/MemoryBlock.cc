#include <neto/mm/MemoryBlock.h>

#include <string.h>

#include <gtest/gtest.h>

using namespace neto::mm;

#define BLOCK_SIZE 50
#define TEST_STR "This is a test"

TEST(MemoryBlock, alloc) {
  MemoryBlockPtr ptr = MemoryBlock::alloc(BLOCK_SIZE);

  EXPECT_EQ(ptr->size(), BLOCK_SIZE);
  EXPECT_EQ(ptr->shared(), false);
}

TEST(MemoryBlock, share) {
  MemoryBlockPtr ptr = MemoryBlock::alloc(BLOCK_SIZE);

  memcpy(ptr->begin(), TEST_STR, strlen(TEST_STR));

  EXPECT_EQ(ptr->size(), BLOCK_SIZE);
  EXPECT_EQ(ptr->shared(), false);

  MemoryBlockPtr shared = ptr->share();

  EXPECT_EQ(ptr->shared(), true);
  EXPECT_EQ(shared->shared(), true);
  EXPECT_EQ(ptr->begin(), shared->begin());

  EXPECT_EQ(memcmp(ptr->begin(), shared->begin(), strlen(TEST_STR)), 0);
}

TEST(MemoryBlock, copy) {
  MemoryBlockPtr ptr = MemoryBlock::alloc(BLOCK_SIZE);

  memcpy(ptr->begin(), TEST_STR, strlen(TEST_STR));

  EXPECT_EQ(ptr->size(), BLOCK_SIZE);
  EXPECT_EQ(ptr->shared(), false);

  MemoryBlockPtr shared = ptr->copy();

  EXPECT_EQ(ptr->shared(), false);
  EXPECT_EQ(shared->shared(), false);
  EXPECT_NE(ptr->begin(), shared->begin());

  EXPECT_EQ(memcmp(ptr->begin(), shared->begin(), strlen(TEST_STR)), 0);
}

TEST(MemoryBlock, split) {
  MemoryBlockPtr ptr = MemoryBlock::alloc(BLOCK_SIZE);

  memcpy(ptr->begin(), TEST_STR, strlen(TEST_STR));

  EXPECT_EQ(ptr->size(), BLOCK_SIZE);
  EXPECT_EQ(ptr->shared(), false);

  MemoryBlockPtr shared = ptr->share();

  shared->split();

  EXPECT_EQ(ptr->shared(), false);
  EXPECT_EQ(shared->shared(), false);

  EXPECT_EQ(memcmp(ptr->begin(), shared->begin(), strlen(TEST_STR)), 0);
}
