#pragma once

#ifdef WIN32

#pragma comment(lib, "winmm.lib")

#include "../singleton.h"
#include "ttime.h"
#include <mmsystem.h>
#include <windows.h>

namespace beam {

inline ttime get_performance_time() {
  LARGE_INTEGER t, f;
  QueryPerformanceFrequency(&f);
  QueryPerformanceCounter(&t);
  return ttime(t.QuadPart * 1000000 / f.QuadPart);
}

inline ttime get_steady_time() {
  return ttime(timeGetTime() * 1000);
}

enum time_method {
  TIME_METHOD_STEADY_TIME,
  TIME_METHOD_PERFORMANCE_TIME
};

namespace ttime_detail {

class time_method_holder {
  friend ttime get_time();
  friend void set_time_method(time_method);
  time_method m_;
};

}  // namespace beam::ttime_detail

inline ttime get_time() {
  time_method m = singleton<ttime_detail::time_method_holder>::get().m_;
  if (m == TIME_METHOD_STEADY_TIME) return get_steady_time();
  return get_performance_time();
}

inline void set_time_method(time_method m) {
  singleton<ttime_detail::time_method_holder>::get().m_ = m;
}

}  // namespace beam

#endif
