#pragma once

// TODO: Use proper macro to distinguish POSIX.
#ifdef __GNUC__

#include <sys/time.h>
#include "ttime.h"

namespace beam {

inline ttime get_performance_time() {
  timeval tv;
  gettimeofday(&tv, 0);
  return ttime(tv.tv_sec * 1000000 + tv.tv_usec);
}

inline ttime get_steady_time() {
  return get_performance_time();
}

inline ttime get_time() {
  return get_performance_time();
}

enum get_time_method {
  GET_STEADY_TIME,
  GET_PERFORMANCE_TIME
};

inline void set_time_method(get_time_method) {}

}  // namespace beam

#endif
