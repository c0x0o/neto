#ifndef __NETO_BASE_TIMESTAMP_H__
#define __NETO_BASE_TIMESTAMP_H__

#include <time.h>
#include <stdio.h>
#include <string.h>

#include <string>

namespace neto {
namespace base {

  const char *MonthString[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
    "Aug", "Sep", "Oct", "Nov", "Dec"};
  const char *WeekString[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  const char GMTfmt[] = "%.3s, %.02d %.3s %.04d %.02d:%.02d:%.02d %s";

  class Timestamp {
    public:
      static Timestamp now();

      Timestamp(unsigned long timeSinceEpoch = 0);
      Timestamp(struct timespec *spec);
      ~Timestamp() {};

      std::string toNumericString(unsigned int reserve = 4) const;
      std::string toUTCString() const;

      int getYear() const;
      int getMonth() const;
      int getDate() const;
      int getDay() const;
      int getDayOfYear() const;
      int getHour() const;
      int getMinute() const;
      int getSecond() const;

      bool operator>(const Timestamp &) const;
      bool operator>=(const Timestamp &) const;
      bool operator<(const Timestamp &) const;
      bool operator<=(const Timestamp &) const;
      bool operator==(const Timestamp &) const;

      Timestamp &operator=(const Timestamp &);

    private:
      struct timespec spec_;
      struct tm tm_;
  };

} // namespace base
} // namespace neto

#endif // __NETO_BASE_MUTEX_H__
