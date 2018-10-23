#ifndef __NETO_BASE_THREAD_H__
#define __NETO_BASE_THREAD_H__

#include <memory>
#include <functional>

#include <pthread.h>
#include <string.h>
#include <signal.h>

namespace neto {
namespace base {
  class Thread;
  class ThreadInfo;

  typedef std::shared_ptr<Thread> ThreadPtr;
  typedef std::function<void()> Runnable;
  typedef pthread_t tid;

  class Thread {
    public:
      static ThreadPtr create(const Runnable &functor);
      static bool inThread(ThreadPtr &thread);

      virtual ~Thread();

      tid id() const {
        return id_;
      }
      bool alive() const {
        return 0 == pthread_kill(id_, 0);
      }
      bool detached() const;

      int detach();
      int join(void **retval = NULL);
      int cancel();

    private:
      static void *main(void *);

      Thread(tid id, const Runnable &functor);
      Thread();
      Thread(const Thread &);
      Thread(const Thread &&);

      pthread_t id_;
      Runnable main_;
  };

  class ThreadInfo {
    public:
      static ThreadInfo &current();

      ThreadPtr thread();
      int setReferer(void *);
    private:
      ThreadInfo();
      ~ThreadInfo();

      pthread_key_t referers_;
  };

} // namespace neto
} // namespace base

#endif // __NETO_BASE_THREAD_H__
