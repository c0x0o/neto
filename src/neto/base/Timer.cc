#include <neto/base/Timer.h>

using namespace neto::base;

Timer::Timer(const Timestamp &expire)
  : startAt_(),
    expireAt_(expire),
    counter_(0),
    alive_(false),
    interval_(0)
{
}

Timer::Timer(float interval)
  : startAt_(),
    expireAt_(),
    counter_(0),
    alive_(false),
    interval_(interval)
{
}

bool Timer::expired(const Timestamp &competitor) const {
  if (competitor >= expireAt_) {
    return true;
  }

  return false;
}
bool Timer::alive() const {
  return alive_;
}
const Timestamp &Timer::startAt() const {
  return startAt_;
}
const Timestamp &Timer::nextExpireTime() const {
  return expireAt_;
}
unsigned long Timer::count() const {
  return counter_;
}

void Timer::setExpireCallback(const TimerCallback &cb) {
  expireCallback_ = cb;
}
void Timer::setClearCallback(const TimerCallback &cb) {
  clearCallback_ = cb;
}

void Timer::init(const Timestamp &start) {
  startAt_ = start;
  alive_ = true;

  if (interval_ != 0) {
    // set by interval
    expireAt_ = startAt_ + interval_;
  }
}

void Timer::clear() {
  alive_ = false;
}

int Timer::trigger(const Timestamp &competitor) {
  if (!alive_) {
    if (clearCallback_) {
      clearCallback_(*this);
    }
    // indicate that Timer should be destroyed right now
    return -1;
  }

  if (expired(competitor)) {
    if (expireCallback_) {
      expireCallback_(*this);
    }
  } else {
    return 0;
  }

  if (interval_ != 0) {
    // set by interval
    expireAt_ = competitor + interval_;
  } else {
    // Timer will be destroyed in next tick
    alive_ = false;
  }

  counter_++;

  return 0;
}

bool Timer::operator<(const Timer &timer) const {
  if (!alive_) {
    // a non-alive Timer is always at the last of queue
    return false;
  }

  return this->expireAt_ < timer.expireAt_;
}
bool Timer::operator>(const Timer &timer) const {
  if (!alive_) {
    return true;
  }

  return this->expireAt_ > timer.expireAt_;
}
