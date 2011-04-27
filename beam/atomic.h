#pragma once

#ifdef _WIN32
# include <windows.h>
#elif defined(__APPLE__) && __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= 1050
# define BEAM_ATOMIC_CAS_USE_MACOSX_OSATOMIC
# include <libkern/OSAtomic.h>
#elif __GNUC__ * 100 + __GNUC_MINOR__ >= 401
# define BEAM_ATOMIC_CAS_USE_GNUC_EXTENSION
#else
# include <pthread.h>
# define BEAM_MEMORY_BARRIER __asm__ __volatile__ ("" ::: "memory")
#endif

namespace beam {

inline int compare_and_swap(volatile int& val, int oldval, int newval) {
#ifdef _WIN32
  return InterlockedCompareExchange(&val, newval, oldval);
#elif defined(BEAM_ATOMIC_CAS_USE_MACOSX_OSATOMIC)
  return OSAtomicCompareAndSwapIntBarrier(oldval, newval, &val) ? oldval : val;
#elif defined(BEAM_ATOMIC_CAS_USE_GNUC_EXTENSION)
  return __sync_val_compare_and_swap(&val, oldval, newval);
#else
  static pthread_mutex_t pmutex = PTHREAD_MUTEX_INITIALIZER;
  int ret = newval;
  int tmpval = val;
  BEAM_MEMORY_BARRIER;
  if (tmpval == oldval) {
    pthread_mutex_lock(&pmutex);
    tmpval = val;
    if (tmpval == oldval) {
      tmpval = newval;
      BEAM_MEMORY_BARRIER;
      val = tmpval;
      ret = oldval;
    }
    pthread_mutex_unlock(&pmutex);
  }
  return ret;
#endif
}

typedef volatile int once_flag;
static const once_flag ONCE_INIT = 0;
static const once_flag ONCE_CALLED = 1;

template <typename F> inline void call_once(once_flag& flag, F function) {
  if (compare_and_swap(flag, ONCE_INIT, ONCE_CALLED) == ONCE_INIT) {
    function();
  }
}

}  // namespace beam
