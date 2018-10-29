#include <neto/base/Mutex.h>

#include <gtest/gtest.h>
#include <unistd.h>

#include <neto/base/Thread.h>

using namespace neto::base;

static struct timespec interval_long = {0, 300000000}; // 0.3s
static struct timespec interval_short = {0, 100000000}; // 0.1s
#define SLEEP_LONG() nanosleep(&interval_long, NULL)
#define SLEEP_SHORT() nanosleep(&interval_short, NULL)

TEST(Mutex, BasicOperation) {
  int counter = 0;
  Mutex lock;
  Thread thread([&counter, &lock]() {
        lock.lock();
        SLEEP_LONG();
        counter = 2;
        lock.unlock();
      });
  SLEEP_SHORT();
  if (!lock.locked()) {
    counter = 1;
    EXPECT_EQ(1,2);
  }
  thread.join();
  EXPECT_EQ(counter, 2);
}

TEST(Mutex, Robust) {
  Mutex lock;
  int ret;
  Thread thread([&lock]() {
        lock.lock();
      });

  thread.join();

  // lock failed because of EOWNERDEAD
  ret = lock.lock();
  ASSERT_EQ(errno, EOWNERDEAD);
  ASSERT_EQ(ret, -EOWNERDEAD);
  ASSERT_EQ(lock.locked(), false);

  // try again
  ret = lock.lock();
  ASSERT_EQ(ret, 0);
  ASSERT_EQ(lock.locked(), true);

  ret = lock.unlock();
  ASSERT_EQ(ret, 0);
}


TEST(MutexGuard, constructor) {
  Mutex lock;
  Thread thread([&lock]() {
      MutexGuard guard(lock);
      SLEEP_LONG();
      });
  SLEEP_SHORT();
  ASSERT_EQ(lock.locked(), true);
  thread.join();
  ASSERT_EQ(lock.locked(), false);
}
