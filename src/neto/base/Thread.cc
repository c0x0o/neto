#include <neto/base/Thread.h>

using namespace neto::base;
using std::make_shared;

ThreadPtr MainThread::main_ = make_shared<Thread>();

ThreadPtr Thread::create(const Runnable &functor) {
  return make_shared<Thread>(functor);
}

// get a Thread which represent current thread
// users are not expect to use this constructor
Thread::Thread()
  : id_(pthread_self()),
    main_()
{
}
// only available through Thread::create
Thread::Thread(const Runnable &functor)
  : id_(0),
    main_(functor)
{
  int ret = 0;
  pthread_attr_t attr;

  pthread_attr_init(&attr);
  ret = pthread_create(&id_, &attr, &Thread::main, new Runnable(functor));
  if (ret != 0) {
    id_ = 0;
  }
  pthread_attr_destroy(&attr);
}

Thread::~Thread() {
  void *ret;

  // TODO
  // 1. need more graceful way to stop the thread, such as signal
  if (!isMainThread()) {
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
  return id() == MainThread::id();
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

int Thread::kill(int sig) {
  return pthread_kill(id_, sig);
}

int Thread::cancel() {
  if (isMainThread()) {
    return -1;
  }

  return pthread_cancel(id_);
}

void *Thread::main(void *arg) {
  Runnable runner = *static_cast<Runnable *>(arg);

  delete static_cast<Runnable *>(arg);

  runner();

  return NULL;
}

