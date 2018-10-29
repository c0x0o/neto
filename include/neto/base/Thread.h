#ifndef __NETO_BASE_THREAD_H__
#define __NETO_BASE_THREAD_H__

#include <memory>
#include <functional>

#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <sched.h>

namespace neto {
namespace base {
  class Thread;
  class CurrentThread;
  class MainThread;

  typedef std::shared_ptr<Thread> ThreadPtr;
  typedef std::weak_ptr<Thread> ThreadWeakPtr;
  typedef std::function<void()> Runnable;
  typedef pthread_t tid;

  class Thread {
    public:
      static ThreadPtr create(const Runnable &functor);

      Thread();
      Thread(const Runnable &functor);
      virtual ~Thread();

      tid id() const {
        return id_;
      }
      bool alive() const {
        return 0 == pthread_kill(id_, 0);
      }
      bool detached() const;
      bool isMainThread() const;

      int detach();
      int join(void **retval = NULL);
      int kill(int sig = 0);
      int cancel();

    private:
      static void *main(void *);

      Thread(const Thread &) = delete;
      Thread(const Thread &&) = delete;

      pthread_t id_;
      Runnable main_;
  };

  class MainThread {
    public:
      static tid id() {return main_->id();}
      static ThreadPtr thread() {return main_;}
    private:
      static ThreadPtr main_;
  };

  class CurrentThread {
    public:
      static tid id() {return pthread_self();}
      static bool isCurrentThread(const Thread &t) {return id() == t.id();}
      static bool isMainThread() {return id() == MainThread::id();}
      static void enableCancel() {
        int old;
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old);
      }
      static void disableCancel() {
        int old;
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old);
      }
      static void yield() {
        sched_yield();
      }
    private:
      static tid mainId_;
      static Thread mainThread;
  };

} // namespace neto
} // namespace base

#endif // __NETO_BASE_THREAD_H__
