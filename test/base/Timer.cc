#include <neto/base/Timer.h>

#include <gtest/gtest.h>

using namespace neto::base;

TEST(Timer, contructor) {
  Timestamp now(Timestamp::now());
  Timer expireNow(now);
  Timer expireTomorrow(now + (24 * 60 * 60));

  EXPECT_EQ(expireTomorrow.expired(now), false);
  EXPECT_EQ(expireNow.expired(now), true);
  EXPECT_EQ(expireNow.alive(), false);
  EXPECT_EQ(expireNow.nextExpireTime(), now);
  EXPECT_EQ(expireNow.count(), 0);
}

TEST(Timer, callback) {
  Timestamp now(Timestamp::now());
  Timer expireNow(now);
  int counter = 0, ret;

  expireNow.init(now);
  expireNow.setExpireCallback([&counter](Timer &) {
      counter++;
      });
  expireNow.setClearCallback([&counter](Timer &) {
      counter++;
      });

  ret = expireNow.trigger(now);
  EXPECT_EQ(ret, 0);
  EXPECT_EQ(counter, 1);
  ret = expireNow.trigger(now);
  EXPECT_EQ(ret, -1);
  EXPECT_EQ(counter, 2);
}

TEST(Timer, init) {
  Timestamp now(Timestamp::now()), epoch;
  Timer expireNow(now);

  expireNow.init(now);

  EXPECT_EQ(expireNow.startAt(), now);
  EXPECT_EQ(expireNow.alive(), true);

  ASSERT_EQ(expireNow.expired(now), true);

  expireNow.trigger(now);

  EXPECT_EQ(expireNow.alive(), false);
  EXPECT_EQ(expireNow.count(), 1);
  EXPECT_EQ(expireNow.nextExpireTime(), now);
}

TEST(Timer, setByInterval) {
  Timer intervalTimer(1);
  Timestamp now(Timestamp::now()), oneSecLater(now+1), twoSecLater(now + 2);

  intervalTimer.init(now);

  EXPECT_EQ(intervalTimer.nextExpireTime(), oneSecLater);
  EXPECT_EQ(intervalTimer.expired(now), false);
  EXPECT_EQ(intervalTimer.alive(), true);

  EXPECT_EQ(intervalTimer.expired(oneSecLater), true);

  ASSERT_EQ(intervalTimer.expired(oneSecLater), true);
  intervalTimer.trigger(oneSecLater);

  EXPECT_EQ(intervalTimer.count(), 1);
  EXPECT_EQ(intervalTimer.nextExpireTime(), twoSecLater);
  EXPECT_EQ(intervalTimer.alive(), true);
}
