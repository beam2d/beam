#pragma once

#include <boost/date_time/posix_time/posix_time_types.hpp>
#ifdef WIN32
# pragma comment(lib, "winmm.lib")
# include <mmsystem.h>
# include <windows.h>
# include <boost/thread/locks.hpp>
# include <boost/thread/mutex.hpp>
# include "singleton.h"
#endif

namespace beam {

enum time_method {
  TIME_METHOD_TIME_GET_TIME,
  TIME_METHOD_QUERY_PERFORMANCE_COUNTER,
  TIME_METHOD_MICROSEC_CLOCK
};

#ifndef WIN32

// microsec_clock uses gettimeofday on POSIX, which achieves microsecond
// resolution. It does not on Windows (Boost 1.46.1).
inline boost::posix_time::ptime ptime() {
  return boost::posix_time::microsec_clock::local_time();
}

inline time_method ptime_method() { return TIME_METHOD_MICROSEC_CLOCK; }
inline void ptime_method(time_method) {}
inline void time_period(unsigned int) {}

#else

namespace ptime_win32_detail {

class high_performance_timer : public singleton<high_performance_timer> {
  friend class singleton<high_performance_timer>;

  boost::mutex mutex_;
  time_method method_;
  UINT period_;

  boost::posix_time::ptime start_ptime_;
  DWORD start_tgt_;
  LARGE_INTEGER start_qpc_;
  LARGE_INTEGER qpf_;

  high_performance_timer() : period_(0) {
    // This value is not precise enough, but we use it as base time.
    start_ptime_ = boost::posix_time::microsec_clock::local_time();

    start_tgt_ = timeGetTime();
    QueryPerformanceFrequency(&start_qpf_);
    QueryPerformanceCounter(&start_qpc_);

    // Simply distinguish either QueryPerformanceCounter uses RDTSC or not.
    // This routine does not always distinguish correctly.
    long long err = __rdtsc() - start_qpc_.QuadPart;
    if (err >= 0 && err < (1LL << 30)) {  // 1LL << 30 means 1sec on 1 GHz CPU.
      // QueryPerformanceCounter seems to use RDTSC, so do not use it.
      method_ = TIME_METHOD_TIME_GET_TIME;
    } else {
      // QueryPerformanceCounter seems not to use RDTSC.
      // Other candidates are: PIT, ACPI or HPET
      method_ = TIME_METHOD_QUERY_PERFORMANCE_COUNTER;
    }
  }

  ~high_performance_timer() {
    if (period_) {
      timeEndPeriod(period_);
    }
  }

 public:
  time_method method() const { return method_; }
  void method(time_method m) { method_ = m; }

  void time_period(UINT period) {
    boost::lock_guard<boost::mutex> lock(mutex_);
    if (period_) timeEndPeriod(period_);
    if (period) timeBeginPeriod(period);
    period_ = period;
  }

  boost::posix_time::ptime time() const {
    switch (method_) {
      case TIME_METHOD_TIME_GET_TIME:
        return start_ +
            boost::posix_time::milliseconds(timeGetTime() - start_tgt_);
      case TIME_METHOD_QUERY_PERFORMANCE_COUNTER: {
        LONG_INTEGER now;
        QueryPerformanceCounter(&now);
        return start_ + boost::posix_time::microseconds(
            (now.QuadPart - start_qpc_.QuadPart) * 1000000 / qpf_.QuadPart);
      }
      default:
        return boost::posix_time::microsec_clock::local_time();
    }
  }
};

}  // namespace ptime_win32_detail

inline boost::posix_time::ptime ptime() {
  return ptime_win32_detail::high_performance_timer::get().get_time();
}

inline void ptime_method(time_method m) {
  ptime_win32_detail::high_performance_timer::get().method(m);
}

inline time_method ptime_method() {
  return ptime_win32_detail::high_performance_timer::get().method();
}

inline void time_period(unsigned int p) {
  ptime_win32_detail::high_performance_timer::get().time_period(p);
}

#endif

}  // namespace beam
