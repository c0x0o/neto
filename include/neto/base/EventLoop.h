#ifndef __NETO_BASE_EVENTLOOP_H__
#define __NETO_BASE_EVENTLOOP_H__

#include <errno.h>
#include <sys/epoll.h>

#include <queue>
#include <deque>
#include <memory>
#include <atomic>
#include <map>
#include <functional>

#include <neto/base/Thread.h>
#include <neto/base/Mutex.h>
#include <neto/base/Spinlock.h>
#include <neto/base/Timestamp.h>
#include <neto/base/Timer.h>
#include <neto/base/PtrCompare.h>

namespace neto {
namespace base {
  class EventLoop;

  typedef std::shared_ptr<EventLoop> EventLoopPtr;

  // EventLoop may be accessed by multiple threads
  // Phase 1: execute Timer
  // Phase 2: execute pending functor(add by nextTick)
  // Phase 3: poll
  // Phase 4: execute setImmediate functor
  class EventLoop {
    public:
      typedef enum {RUNNING, PAUSE, QUIT} Status;

      // call without any args will create a new thread
      explicit EventLoop();
      // user should pass the MainThread::thread()
      // or the loop will never be executed
      explicit EventLoop(ThreadPtr &mainThread);
      virtual ~EventLoop();

      Status status() const {return status_;}

      int setImmediate(const Runnable &functor);
      int nextTick(const Runnable &functor);
      TimerWeakPtr setTimeout(float timeout, const TimerCallback &functor);
      TimerWeakPtr setInterval(float interval, const TimerCallback &functor);

      ThreadWeakPtr hostThread() {return ThreadWeakPtr(host_);}
      bool hostAlive() const {return host_->alive();}

      void loop();
      void quit();
      void pause();
    private:
      EventLoop(const EventLoop &) = delete;
      EventLoop(const EventLoop &&) = delete;

      void loopInThread();

      bool inHostThread() const;

      std::atomic<Status> status_;
      ThreadPtr host_;
      // functor queues
      Spinlock timerLock_;
      std::priority_queue<TimerPtr,
                          std::deque<TimerPtr>,
                          neto::base::PtrGreater<TimerPtr>> timers_;
      Spinlock functorLock_;
      std::queue<Runnable> functors_;
      Spinlock immediateLock_;
      std::queue<Runnable> immediates_;
      // TODO
      // 1. poller
      // 2. channel
  };

} // namespace base
} // namespace neto

#endif // __NETO_BASE_EVENTLOOP_H__

