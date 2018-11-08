#include <neto/base/EventLoop.h>

using namespace neto::base;
using std::make_shared;
using std::priority_queue;
using std::queue;

EventLoop::EventLoop()
  : status_(PAUSE),
    timers_(),
    functors_(),
    immediates_(),
    epfd_(epoll_create(1)),
    channels_()
{
  host_ = Thread::create(std::bind(&EventLoop::loopInThread, this));
}

EventLoop::EventLoop(ThreadPtr &mainThread)
  : status_(PAUSE),
    host_(mainThread),
    timers_(),
    functors_(),
    immediates_(),
    epfd_(epoll_create(1)),
    channels_()
{
}

EventLoop::~EventLoop() {
  quit();

  if (host_->alive() && !host_->isMainThread()) {
    host_->join();
  }
}

bool EventLoop::inHostThread() const {
  return CurrentThread::isCurrentThread(*host_);
}

int EventLoop::setImmediate(const Runnable &functor) {
  if (!inHostThread()) {
    SpinlockGuard guard(immediateLock_);

    immediates_.push(functor);
  } else {
    immediates_.push(functor);
  }

  return 0;
}

int EventLoop::nextTick(const Runnable &functor) {
  if (!inHostThread()) {
    SpinlockGuard guard(functorLock_);

    functors_.push(functor);
  } else {
    functors_.push(functor);
  }

  return 0;
}

TimerWeakPtr EventLoop::setTimeout(
    float timeout,
    const TimerCallback &functor)
{
  Timestamp expire = Timestamp::now();

  expire += timeout;

  TimerPtr timer = make_shared<Timer>(expire);

  timer->setExpireCallback(functor);
  timer->init(Timestamp::now());

  if (!inHostThread()) {
    SpinlockGuard guard(timerLock_);
    timers_.push(timer);
  } else {
    timers_.push(timer);
  }

  return timer;
}

TimerWeakPtr EventLoop::setInterval(
    float interval,
    const TimerCallback &functor)
{
  TimerPtr timer = make_shared<Timer>(interval);

  timer->setExpireCallback(functor);
  timer->init(Timestamp::now());

  if (!inHostThread()) {
    SpinlockGuard guard(timerLock_);
    timers_.push(timer);
  } else {
    timers_.push(timer);
  }

  return timer;
}

int EventLoop::attachEventChannel(const EventChannelPtr &ptr) {
  return nextTick(
    [&ptr, this]() {
      struct epoll_event ev;

      channels_[ptr->fd()] = ptr;
      ev.events = ptr->events();
      ev.data.fd = ptr->fd();

      epoll_ctl(epfd_, EPOLL_CTL_ADD, ptr->fd(), &ev);
    });
}

int EventLoop::dettachEventChannel(int fd) {
  return nextTick(
     [fd, this]() {
       struct epoll_event ev;

       ev.data.fd = fd;

       epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, &ev);
       channels_.erase(fd);
     });
}


void EventLoop::loop() {
  status_ = RUNNING;

  if (host_->isMainThread()) {
    loopInThread();
  }

  return;
}
void EventLoop::quit() {
  status_ = QUIT;
}
void EventLoop::pause() {
  status_ = PAUSE;
}

void EventLoop::loopInThread() {
  while (status_ != QUIT) {

    if (status_ == PAUSE) {
      CurrentThread::yield();
      continue;
    }

    Timestamp startTime = Timestamp::now();
    queue<TimerPtr> timers;
    queue<TimerPtr> again;
    queue<Runnable> functors;

    // phase 1: Timer
    {
      SpinlockGuard guard(timerLock_);
      // collect expired timer
      while (timers_.size()) {
        TimerPtr top = timers_.top();
        if (!top->expired(startTime)) break;
        timers.push(top);
        timers_.pop();
      }
    }
    // execute timer callback
    // dead timer will be destroyed
    while (timers.size()) {
      int ret = 0;
      TimerPtr top = timers.front();
      timers.pop();
      ret = top->trigger(startTime);
      if (ret == 0) {
        // Timer should be add back to timer queue
        again.push(top);
      }
    }
    // add alive timer(set by interval) back to timer queue
    // outdated timer will be added back and destroyed in next tick
    {
      SpinlockGuard guard(timerLock_);
      while (again.size()) {
        timers_.push(again.front());
        again.pop();
      }
    }

    // phase 2: Pending functor(add by nextTick in last tick)
    {
      SpinlockGuard guard(functorLock_);
      functors.swap(functors_);
    }
    while (functors.size()) {
      Runnable func = functors.front();
      func();
      functors.pop();
    }

    // phase 3: Poll
    struct epoll_event events[MaxEpollEvents];
    int num = epoll_wait(epfd_, events, MaxEpollEvents, 0);
    if (num < 0) {
      if (errno != EAGAIN && errno != EWOULDBLOCK) {
        status_ = QUIT;
      } else {
        num = 0;
      }
    }
    // order should not be changed before considerating
    for (int i = 0; i < num; i++) {
      EventChannelPtr channel = channels_.at(events[i].data.fd);

      if (channel->disabled()) {
        continue;
      }

      if (events[i].events & EPOLLERR) {
        channel->handleERR();
      }
      if (events[i].events & EPOLLHUP) {
        channel->handleHUP();
      }
      if (events[i].events & EPOLLRDHUP) {
        channel->handleRDHUP();
      }
      if (events[i].events & EPOLLPRI) {
        channel->handlePRI();
      }
      if (events[i].events & EPOLLIN) {
        channel->handleIN();
      }
      if (events[i].events & EPOLLOUT) {
        channel->handleOUT();
      }
    }

    // phase 4: Immediate functor(add by setImmediate in this tick)
    {
      SpinlockGuard guard(immediateLock_);
      functors.swap(immediates_);
    }
    while (functors.size()) {
      Runnable func = functors.front();
      func();
      functors.pop();
    }
  }
}



