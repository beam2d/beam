#pragma once

#ifdef _WIN32
# include <windows.h>
#else
# include <pthread.h>
#endif
#include "safe_bool.h"

namespace beam {

class mutex {
#ifdef _WIN32
  CRITICAL_SECTION critical_section_;
#else
  pthread_mutex_t pthread_mutex_;
#endif

 public:
  mutex() {
#ifdef _WIN32
    InitializeCriticalSection(&critical_section_);
#else
    pthread_mutex_init(&pthread_mutex_, 0);
#endif
  }

  ~mutex() {
#ifdef _WIN32
    DeleteCriticalSection(&critical_section_);
#else
    pthread_mutex_destroy(&pthread_mutex_);
#endif
  }

  void lock() {
#ifdef _WIN32
    EnterCriticalSection(&critical_section_);
#else
    pthread_mutex_lock(&pthread_mutex_);
#endif
  }

  void unlock() {
#ifdef _WIN32
    LeaveCriticalSection(&critical_section_);
#else
    pthread_mutex_unlock(&pthread_mutex_);
#endif
  }
};

struct dummy_mutex {
  void lock() {}
  void unlock() {}
};

template <typename MutexT = mutex> class scoped_lock {
  mutex& mutex_;

 public:
  explicit scoped_lock(mutex& m) : mutex_(m) {
    m.lock();
  }

  ~scoped_lock() {
    mutex_.unlock();
  }
};

template <> struct scoped_lock<dummy_mutex> {
  explicit scoped_lock(dummy_mutex&) {}
};

}  // namespace beam
