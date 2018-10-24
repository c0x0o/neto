#include <neto/base/Thread.h>

using namespace neto::base;

// init ThreadInfo
Thread::ThreadInfo Thread::info_;

ThreadPtr Thread::create(const Runnable &functor) {
  pthread_attr_t attr;
  int ret = 0;
  ThreadPtr ptr(new Thread(0, functor));

  pthread_attr_init(&attr);

  ret = pthread_create(&ptr->id_, &attr, &Thread::main, new ThreadPtr(ptr));

  pthread_attr_destroy(&attr);

  if (ret != 0) {
    return ThreadPtr(NULL);
  }

  return ptr;
}

bool Thread::inThread(ThreadPtr &thread) {
  return Thread::current().thread() == thread;
}

// only for main thread
Thread::Thread()
  : id_(pthread_self()),
    main_(),
    isMainThread_(true)
{
}
// only available through Thread::create
Thread::Thread(tid id, const Runnable &functor)
  : id_(id),
    main_(functor),
    isMainThread_(false)
{
}
Thread::~Thread() {
  void *ret;

  // TODO
  // 1. need more graceful way to stop the thread, such as signal
  if (!isMainThread_) {
    cancel();
    join(&ret);
  }
}


bool Thread::detached() const {
  pthread_attr_t attr;
  int ret = 0;

  ret = pthread_getattr_np(id_, &attr);
  if (ret < 0) {
    return false;
  }

  pthread_attr_getdetachstate(&attr, &ret);
  if (ret != PTHREAD_CREATE_DETACHED) {
    return false;
  }

  return true;
}

bool Thread::isMainThread() const {
  return isMainThread_;
}

int Thread::detach() {
  if (detached()) {
    return 0;
  }

  return pthread_detach(id_);
}

int Thread::join(void **retval) {
  return pthread_join(id_, retval);
}

int Thread::cancel() {
  return pthread_cancel(id_);
}

void *Thread::main(void *arg) {
  ThreadPtr *ptr = static_cast<ThreadPtr *>(arg);
  Thread::current().setReferer(ptr);

  (*ptr)->main_();

  return NULL;
}

Thread::ThreadInfo &Thread::current() {
  return info_;
}

Thread::ThreadInfo::ThreadInfo() {
  pthread_key_create(&referers_, [](void *ptr){
      delete (ThreadPtr *)ptr;
      });

  setReferer(new ThreadPtr(new Thread));
}
Thread::ThreadInfo::~ThreadInfo() {
  pthread_key_delete(referers_);
}

ThreadPtr Thread::ThreadInfo::thread() {
  ThreadPtr *ptr;

  ptr = static_cast<ThreadPtr *>(pthread_getspecific(referers_));

  return ThreadPtr(*ptr);
}

int Thread::ThreadInfo::setReferer(void *refer) {
  return pthread_setspecific(referers_, refer);
}
