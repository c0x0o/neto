#include <neto/base/Timestamp.h>

#define FLOAT_INT(x) static_cast<time_t>(x)
#define FLOAT_NANO(x) static_cast<long>(((x)-FLOAT_INT(x))*1000000000)

using namespace neto::base;
using std::string;
using neto::base::MonthString;
using neto::base::WeekString;
using neto::base::GMTfmt;

const char *neto::base::MonthString[] = {"Jan", "Feb", "Mar", "Apr", "May",
  "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
const char *neto::base::WeekString[] = {"Sun", "Mon", "Tue", "Wed", "Thu",
  "Fri", "Sat"};
const char neto::base::GMTfmt[]
  = "%.3s, %.02d %.3s %.04d %.02d:%.02d:%.02d %s";

Timestamp Timestamp::now() {
  struct timespec spec;

  memset(&spec, 0, sizeof spec);
  clock_gettime(CLOCK_REALTIME, &spec);

  return Timestamp(spec);
}

Timestamp::Timestamp(unsigned long timeSinceEpoch) {
  spec_.tv_sec = timeSinceEpoch;
  spec_.tv_nsec = 0;
  settm();
}
Timestamp::Timestamp(struct timespec &spec) {
  spec_.tv_sec = spec.tv_sec;
  spec_.tv_nsec = spec.tv_nsec;
  settm();
}

void Timestamp::settm() {
  tzset();
  localtime_r(&spec_.tv_sec, &tm_);
}

string Timestamp::toNumericString(unsigned int reserve) const {
  char str[32];
  char fmt[16];
  unsigned long int mod = 1e9;

  if (reserve >  10) {
    return string();
  }

  sprintf(fmt, "%%lu.%%0.%dld", reserve);

  if (reserve == 0) {
    sprintf(str, "%lu", spec_.tv_sec);
  } else {
    while (reserve > 0) {
      mod /= 10;
      reserve--;
    }
    sprintf(str, fmt, spec_.tv_sec, spec_.tv_nsec / mod);
  }

  return string(str);
}

string Timestamp::toUTCString() const {
  char str[40];

  sprintf(str, GMTfmt, WeekString[tm_.tm_wday], tm_.tm_mday,
      MonthString[tm_.tm_mon], tm_.tm_year+1900, tm_.tm_hour, tm_.tm_min,
      tm_.tm_sec, tm_.tm_zone);

  return string(str);
}

int Timestamp::getYear() const {
  return tm_.tm_year + 1900;
}

int Timestamp::getMonth() const {
  return tm_.tm_mon;
}

int Timestamp::getDate() const {
  return tm_.tm_mday;
}

int Timestamp::getDay() const {
  return tm_.tm_wday;
}

int Timestamp::getDayOfYear() const {
  return tm_.tm_yday;
}

int Timestamp::getHour() const {
  return tm_.tm_hour;
}

int Timestamp::getMinute() const {
  return tm_.tm_min;
}

int Timestamp::getSecond() const {
  return tm_.tm_sec;
}

const struct timespec &Timestamp::getTimespec() const {
  return spec_;
}
const struct tm &Timestamp::gettm() const {
  return tm_;
}

bool Timestamp::operator==(const Timestamp &ts) const {
  if (spec_.tv_sec == ts.spec_.tv_sec
      && spec_.tv_nsec == ts.spec_.tv_nsec) {
    return true;
  }

  return false;
}
bool Timestamp::operator!=(const Timestamp &ts) const {
  if (spec_.tv_sec != ts.spec_.tv_sec
      || spec_.tv_nsec != ts.spec_.tv_nsec) {
    return true;
  }

  return false;
}
bool Timestamp::operator>(const Timestamp &ts) const {
  if (spec_.tv_sec > ts.spec_.tv_sec) {
    return true;
  }
  if (spec_.tv_sec == ts.spec_.tv_sec && spec_.tv_nsec > ts.spec_.tv_nsec) {
    return true;
  }

  return false;
}
bool Timestamp::operator<(const Timestamp &ts) const {
  if (spec_.tv_sec < ts.spec_.tv_sec) {
    return true;
  }
  if (spec_.tv_sec == ts.spec_.tv_sec && spec_.tv_nsec < ts.spec_.tv_nsec) {
    return true;
  }

  return false;
}
bool Timestamp::operator<=(const Timestamp &ts) const {
  if (*this < ts || *this == ts) {
    return true;
  }

  return false;
}
bool Timestamp::operator>=(const Timestamp &ts) const {
  if (*this > ts || *this == ts) {
    return true;
  }

  return false;
}

Timestamp &Timestamp::operator=(const Timestamp &ts) {
  spec_.tv_sec = ts.spec_.tv_sec;
  spec_.tv_nsec = ts.spec_.tv_nsec;
  ::memcpy(&tm_, &ts.tm_, sizeof tm_);

  return *this;
}
Timestamp &Timestamp::operator+=(float sec) {
  spec_.tv_sec += FLOAT_INT(sec);
  spec_.tv_nsec += FLOAT_NANO(sec);
  settm();

  return *this;
}
Timestamp &Timestamp::operator-=(float sec) {
  spec_.tv_sec -= FLOAT_INT(sec);
  spec_.tv_nsec -= FLOAT_NANO(sec);
  settm();

  return *this;
}
Timestamp Timestamp::operator+(float sec) const {
  struct timespec result;
  result.tv_sec = spec_.tv_sec + FLOAT_INT(sec);
  result.tv_nsec = spec_.tv_nsec + FLOAT_NANO(sec);

  return Timestamp(result);
}
Timestamp Timestamp::operator-(float sec) const {
  struct timespec result;
  result.tv_sec = spec_.tv_sec - FLOAT_INT(sec);
  result.tv_nsec = spec_.tv_nsec - FLOAT_NANO(sec);

  return Timestamp(result);
}
