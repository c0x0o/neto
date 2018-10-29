#include <neto/base/Timestamp.h>

#include <gtest/gtest.h>

#include <string>

using namespace neto::base;
using std::string;

TEST(Timestamp, toXXXString) {
  Timestamp epoch;

  struct timespec spec = {0,0};
  struct tm tm;
  tzset();
  localtime_r(&spec.tv_sec, &tm);

  EXPECT_EQ(epoch.toNumericString(), string("0.0000"));
  EXPECT_EQ(epoch.toUTCString(),
            string("Thu, 01 Jan 1970 08:00:00 ")+string(tm.tm_zone));
}

TEST(Timestamp, comparison) {
  int i = 0;
  Timestamp now(Timestamp::now()), cp(now);
  Timestamp later;

  while(i < 1000000) i++;

  later = Timestamp::now();

  EXPECT_EQ(now, cp);
  EXPECT_GT(later, now);
  EXPECT_LT(now, later);
  EXPECT_LE(now, cp);
  EXPECT_GE(now, cp);
  EXPECT_GE(later, now);
  EXPECT_LE(now, later);
}

TEST(Timestamp, operator) {
  Timestamp epoch(0);
  struct tm tm;
  struct timespec spec = {0,0};
  tzset();
  localtime_r(&spec.tv_sec, &tm);

  EXPECT_EQ(epoch.toUTCString(),
            string("Thu, 01 Jan 1970 08:00:00 ")+string(tm.tm_zone));
  Timestamp nextday = epoch + (24 * 60 * 60);
  EXPECT_EQ(nextday.toUTCString(),
            string("Fri, 02 Jan 1970 08:00:00 ")+string(tm.tm_zone));
  nextday -= (24 * 60 * 60);
  EXPECT_EQ(nextday.toUTCString(),
            string("Thu, 01 Jan 1970 08:00:00 ")+string(tm.tm_zone));
}
