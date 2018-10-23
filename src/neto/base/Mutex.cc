#include <neto/base/Mutex.h>

using namespace neto::base;

Mutex::Mutex()
  : underlock_(false)
{
  pthread_mutexattr_init(&attr_);
  pthread_mutexattr_setrobust(&attr_, PTHREAD_MUTEX_ROBUST);
  pthread_mutex_init(&mutex_, &attr_);
}

Mutex::~Mutex() {
  pthread_mutex_destroy(&mutex_);
  pthread_mutexattr_destroy(&attr_);
}

bool Mutex::locked() const {
  return underlock_.load() ? true : false;
}

// if return values less than 0 and errno = EOWNERDEAD,
// user should try to acquire lock again
int Mutex::lock() {
  int ret = pthread_mutex_lock(&mutex_);

  if (ret != 0) {
    if (ret == EOWNERDEAD) {
      pthread_mutex_consistent(&mutex_);
      pthread_mutex_unlock(&mutex_);
      underlock_.store(false);
    }

    errno = ret;
  } else {
    underlock_.store(true);
  }

  return -ret;
}

int Mutex::unlock() {
  int ret = 0;

  if (!underlock_.load()) {
    return 0;
  }

  ret = pthread_mutex_unlock(&mutex_);
  if (ret != 0) {
    errno = ret;
    return -ret;
  } else {
    underlock_.store(false);
  }

  return 0;
}

MutexGuard::MutexGuard(Mutex &lock)
  : lock_(lock)
{
  lock_.lock();
}

MutexGuard::~MutexGuard() {
  lock_.unlock();
}
