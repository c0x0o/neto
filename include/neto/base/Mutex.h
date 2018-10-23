#ifndef __NETO_BASE_MUTEX_H__
#define __NETO_BASE_MUTEX_H__

#include <pthread.h>
#include <errno.h>

#include <atomic>

namespace neto {
namespace base {
  class Mutex;
  class MutexGuard;

  class Mutex {
    public:
      Mutex();
      ~Mutex();

      bool locked()  const;

      // if return values equals to EOWNERDEAD,
      // user should try to acquire lock again
      int lock();

      int unlock();
    private:
      Mutex(const Mutex &);
      Mutex(const Mutex &&);

      pthread_mutex_t mutex_;
      pthread_mutexattr_t attr_;
      std::atomic<bool> underlock_;
  };

  class MutexGuard {
    public:
      MutexGuard(Mutex &lock);
      ~MutexGuard();
    private:
      MutexGuard(const MutexGuard &);
      MutexGuard(const MutexGuard &&);

      Mutex &lock_;
  };
} // namespace base
} // namespace neto

#endif // __NETO_BASE_MUTEX_H__
