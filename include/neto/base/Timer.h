#ifndef __NETO_BASE_TIMER_H__
#define __NETO_BASE_TIMER_H__

#include <functional>
#include <memory>
#include <atomic>

#include <neto/base/Timestamp.h>

namespace neto {
namespace base {
  class Timer;

  typedef std::function<void(Timer &timer)> TimerCallback;
  typedef std::shared_ptr<Timer> TimerPtr;
  typedef std::weak_ptr<Timer> TimerWeakPtr;

  // Timer should only be used for internal use.
  // User should use EventLoop instead.
  class Timer {
    public:
      explicit Timer(const Timestamp &expire);
      // interval == 0 will produce a timer never trigger
      // interval < 0 will produce a timer trigger before start time
      explicit Timer(float interval);
      ~Timer() {}

      bool expired(const Timestamp &competitor) const;
      bool alive() const;
      const Timestamp &startAt() const;
      const Timestamp &nextExpireTime() const;
      unsigned long count() const;

      void setExpireCallback(const TimerCallback &cb);
      void setClearCallback(const TimerCallback &cb);

      void init(const Timestamp &start);
      void clear();
      int trigger(const Timestamp &competitor);

      bool operator<(const Timer &) const;
      bool operator>(const Timer &) const;
    private:
      Timestamp startAt_;
      Timestamp expireAt_;
      unsigned long int counter_;
      std::atomic<bool> alive_;
      float interval_;

      TimerCallback expireCallback_;
      TimerCallback clearCallback_;
  };

} // namespace base
} // namespace neto


#endif // __NETO_BASE_TIMER_H__
