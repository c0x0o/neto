#ifndef __NETO_BASE_TIMESTAMP_H__
#define __NETO_BASE_TIMESTAMP_H__

#include <time.h>
#include <stdio.h>
#include <string.h>

#include <string>

namespace neto {
namespace base {
  class Timestamp;

  extern const char *MonthString[];
  extern const char *WeekString[];
  extern const char GMTfmt[];

  class Timestamp {
    public:
      static Timestamp now();

      explicit Timestamp(unsigned long timeSinceEpoch = 0);
      explicit Timestamp(struct timespec &spec);
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

      const struct timespec &getTimespec() const;
      const struct tm &gettm() const;

      bool operator>(const Timestamp &) const;
      bool operator>=(const Timestamp &) const;
      bool operator<(const Timestamp &) const;
      bool operator<=(const Timestamp &) const;
      bool operator==(const Timestamp &) const;
      bool operator!=(const Timestamp &) const;

      Timestamp &operator=(const Timestamp &);
      Timestamp &operator+=(float);
      Timestamp &operator-=(float);
      Timestamp operator+(float) const;
      Timestamp operator-(float) const;

    private:
      void settm();

      struct timespec spec_;
      struct tm tm_;
  };

  const Timestamp tsEpoch;

} // namespace base
} // namespace neto

#endif // __NETO_BASE_MUTEX_H__
