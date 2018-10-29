#include <neto/base/Spinlock.h>

#include <unistd.h>

#include <gtest/gtest.h>
#include <neto/base/Thread.h>

using namespace neto::base;

struct timespec spec_short = {0, 100000000}; // 0.1s

TEST(Spinlock, lock) {
  Spinlock lock;
  int counter = 0;
  Thread thread([&counter, &lock]() {
        while (1) {
          lock.lock();
          if (counter<1000) {
            counter++;
          } else {
            lock.unlock();
            break;
          }
          lock.unlock();
        }
        EXPECT_EQ(counter, 1000);
      });

  while (1) {
    lock.lock();
    if (counter<1000) {
      counter++;
    } else {
      lock.unlock();
      break;
    }
    lock.unlock();
  }
  EXPECT_EQ(counter, 1000);
  thread.join();
}

TEST(SpinlockGuard, guard) {
  Spinlock lock;
  int counter = 0;
  Thread thread([&counter, &lock]() {
        while (1) {
          SpinlockGuard guard(lock);
          if (counter<1000) {
            counter++;
          } else {
            break;
          }
        }
        EXPECT_EQ(counter, 1000);
      });

  while (1) {
    SpinlockGuard guard(lock);
    if (counter<1000) {
      counter++;
    } else {
      break;
    }
  }
  EXPECT_EQ(counter, 1000);
  thread.join();
}
