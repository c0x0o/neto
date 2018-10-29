#ifndef __NETO_BASE_SPINLOCK_H__
#define __NETO_BASE_SPINLOCK_H__

#include <atomic>

namespace neto {
namespace base {
  class Spinlock;
  class SpinlockGuard;

  class Spinlock {
    public:
      Spinlock();
      ~Spinlock();

      int lock();
      int unlock();
    private:
      Spinlock(const Spinlock &) = delete;
      Spinlock(const Spinlock &&) = delete;

      std::atomic<bool> locked_;
  };

  class SpinlockGuard {
    public:
      SpinlockGuard(Spinlock &lock);
      ~SpinlockGuard();
    private:
      SpinlockGuard(const SpinlockGuard &) = delete;
      SpinlockGuard(const SpinlockGuard &&) = delete;

      Spinlock &lock_;
  };
}
}

#endif // __NETO_BASE_SPINLOCK_H__
