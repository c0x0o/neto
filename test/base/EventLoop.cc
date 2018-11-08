#include <neto/base/EventLoop.h>

#include <gtest/gtest.h>
#include <time.h>

using namespace neto::base;

struct timespec st = {0, 200000000}; // 0.2s
#define SLEEP_SHORT() nanosleep(&st, NULL)

TEST(EventLoop, nextTick) {
  EventLoop loop;
  int counter = 0;

  ASSERT_EQ(loop.status(), EventLoop::PAUSE);

  loop.nextTick([&counter]() {
      counter = 1;
      });
  loop.loop();
  ASSERT_EQ(loop.status(), EventLoop::RUNNING);
  SLEEP_SHORT();
  EXPECT_EQ(counter, 1);
}

TEST(EventLoop, setImmediate) {
  EventLoop loop;
  int counter = 0;

  ASSERT_EQ(loop.status(), EventLoop::PAUSE);

  loop.nextTick([&counter, &loop]() {
      loop.nextTick([&counter]() {
          counter = 2;
          });
      });
  loop.setImmediate([&counter]() {
      counter = 1;
      });
  loop.loop();
  ASSERT_EQ(loop.status(), EventLoop::RUNNING);
  SLEEP_SHORT();
  EXPECT_EQ(counter, 2);
}

TEST(EventLoop, setTimeout) {
  EventLoop loop;
  int counter = 0;

  loop.loop();

  TimerPtr timer = loop.setTimeout(0.05, [&counter](Timer &){
      counter = 1;
      }).lock();

  SLEEP_SHORT();

  EXPECT_EQ(counter, 1);
  EXPECT_EQ(timer->alive(), false);
}

TEST(EventLoop, setInterval) {
  EventLoop loop;
  int counter = 0;

  loop.loop();

  TimerPtr timer = loop.setInterval(0.05, [&counter](Timer &t){
      counter++;
      if (counter == 2) {
        t.clear();
      }
      }).lock();

  SLEEP_SHORT();

  EXPECT_EQ(counter, 2);
  EXPECT_EQ(timer->alive(), false);
  EXPECT_EQ(timer->count(), 2);
}

TEST(EventLoop, loop) {
  EventLoop loop;

  ASSERT_EQ(loop.status(), EventLoop::PAUSE);
  loop.loop();
  ASSERT_EQ(loop.status(), EventLoop::RUNNING);
  SLEEP_SHORT();
  loop.quit();
  ASSERT_EQ(loop.status(), EventLoop::QUIT);
  loop.hostThread().lock()->join();
  ASSERT_EQ(loop.hostThread().lock()->alive(), false);
}

TEST(EventLoop, mainThread) {
  ThreadPtr main = MainThread::thread();
  EventLoop mainloop(main);
  int counter = 0;

  mainloop.setTimeout(0.1, [&mainloop, &counter](Timer &) {
      counter=1;
      mainloop.quit();
      });

  mainloop.loop();
  ASSERT_EQ(mainloop.status(), EventLoop::QUIT);
}
