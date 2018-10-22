#include <neto/base/Thread.h>

#include <gtest/gtest.h>
#include <unistd.h>

#include <string>

using namespace neto::base;

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
      sleep(1);
      });
  ASSERT_TRUE(ptr->alive());
  ptr->join();
  ASSERT_FALSE(ptr->alive());
}

TEST(Thread, detach) {
  ThreadPtr ptr = Thread::create([]() {
        ::sleep(1);
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
      ::sleep(2);
      });
  ASSERT_TRUE(ptr->alive());
  ptr->cancel();
  ptr->join();
  ASSERT_FALSE(ptr->alive());
}
