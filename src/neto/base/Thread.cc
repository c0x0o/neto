#include <neto/base/Thread.h>

using namespace neto::base;

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
  return ThreadInfo::current().thread() == thread;
}

Thread::Thread(tid id, const Runnable &functor)
  : id_(id),
    main_(functor)
{
}
Thread::~Thread() {
  void *ret;
  cancel();
  join(&ret);
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
  ThreadInfo::current().setReferer(ptr);

  (*ptr)->main_();

  return NULL;
}

ThreadInfo &ThreadInfo::current() {
  static ThreadInfo info;

  return info;
}

ThreadInfo::ThreadInfo() {
  pthread_key_create(&referers_, [](void *ptr){
      delete (ThreadPtr *)ptr;
      });
}
ThreadInfo::~ThreadInfo() {
  pthread_key_delete(referers_);
}

ThreadPtr ThreadInfo::thread() {
  ThreadPtr *ptr;

  ptr = static_cast<ThreadPtr *>(pthread_getspecific(referers_));

  return ThreadPtr(*ptr);
}

int ThreadInfo::setReferer(void *refer) {
  return pthread_setspecific(referers_, refer);
}
