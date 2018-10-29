#include <neto/base/Spinlock.h>

using namespace neto::base;

Spinlock::Spinlock()
  : locked_(false)
{
}
Spinlock::~Spinlock() {}

int Spinlock::lock() {
  bool expect(false);
  while (!locked_.compare_exchange_weak(expect, true,
                                        std::memory_order_release,
                                        std::memory_order_relaxed));

  return 0;
}

int Spinlock::unlock() {
  locked_.store(false);

  return 0;
}

SpinlockGuard::SpinlockGuard(Spinlock &lock)
  : lock_(lock)
{
  lock_.lock();
}

SpinlockGuard::~SpinlockGuard() {
  lock_.unlock();
}
