#include <neto/base/Thread.h>

#include <gtest/gtest.h>
#include <unistd.h>

#include <string>

using namespace neto::base;

// static struct timespec interval_long = {0, 300000000}; // 0.3s
static struct timespec interval_short = {0, 100000000}; // 0.1s
// #define SLEEP_LONG() nanosleep(&interval_long, NULL)
#define SLEEP_SHORT() nanosleep(&interval_short, NULL)

TEST(Thread, defaultConstructor) {
  Thread thread;

  EXPECT_TRUE(thread.isMainThread());
}

TEST(Thread, create) {
  std::string str("Out Side Thread");
  ThreadPtr ptr = Thread::create([&str]() {
        ASSERT_EQ(str, std::string("Out Side Thread"));
      });
  ptr->join();
}

TEST(Thread, inThread) {
  Thread thread([&thread]() {
      ASSERT_TRUE(CurrentThread::isCurrentThread(thread));
      });
  thread.join();
}

TEST(Thread, isMainThread) {
  Thread thread([&thread]() {
      EXPECT_FALSE(thread.isMainThread());
      });
  EXPECT_TRUE(CurrentThread::isMainThread());
  thread.join();
}

TEST(Thread, alive) {
  Thread thread([]() {
      SLEEP_SHORT();
      });
  ASSERT_TRUE(thread.alive());
  thread.join();
  ASSERT_FALSE(thread.alive());
}

TEST(Thread, detach) {
  Thread thread([]() {
      SLEEP_SHORT();
      });
  ASSERT_FALSE(thread.detached());
  thread.detach();
  ASSERT_TRUE(thread.detached());
}

TEST(Thread, join) {
  std::string str;
  Thread thread([&str]() {
      str = "Hi";
      });
  thread.join();
  ASSERT_EQ(str, "Hi");
}

TEST(Thread, cancel) {
  Thread thread([]() {
      CurrentThread::enableCancel();
      // set up a cancel point
      pause();
      });
  SLEEP_SHORT();
  EXPECT_TRUE(thread.alive());
  EXPECT_EQ(thread.cancel(), 0);
  thread.join();
  EXPECT_FALSE(thread.alive());
}
