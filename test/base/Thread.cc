#include <neto/base/Thread.h>

#include <gtest/gtest.h>
#include <unistd.h>

#include <string>

using namespace neto::base;

static struct timespec interval_long = {0, 300000000}; // 0.3s
static struct timespec interval_short = {0, 100000000}; // 0.1s
#define SLEEP_LONG() nanosleep(&interval_long, NULL)
#define SLEEP_SHORT() nanosleep(&interval_short, NULL)

TEST(Thread, create) {
  std::string str("Out Side Thread");
  ThreadPtr ptr = Thread::create([&str]() {
        ASSERT_EQ(str, std::string("Out Side Thread"));
      });
  ptr->join();
}

TEST(Thread, inThread) {
  ThreadPtr ptr = Thread::create([&ptr]() {
      ASSERT_TRUE(Thread::inThread(ptr));
      });
  ptr->join();
}

TEST(Thread, alive) {
  ThreadPtr ptr = Thread::create([]() {
      SLEEP_SHORT();
      });
  ASSERT_TRUE(ptr->alive());
  ptr->join();
  ASSERT_FALSE(ptr->alive());
}

TEST(Thread, detach) {
  ThreadPtr ptr = Thread::create([]() {
      SLEEP_SHORT();
      });
  ASSERT_FALSE(ptr->detached());
  ptr->detach();
  ASSERT_TRUE(ptr->detached());
}

TEST(Thread, join) {
  std::string str;
  ThreadPtr ptr = Thread::create([&str]() {
      str = "Hi";
      });
  ptr->join();
  ASSERT_EQ(str, "Hi");
}

TEST(Thread, cancel) {
  ThreadPtr ptr = Thread::create([]() {
      SLEEP_LONG();
      });
  ASSERT_TRUE(ptr->alive());
  ptr->cancel();
  ptr->join();
  ASSERT_FALSE(ptr->alive());
}
