#pragma once

#include <boost/cstdint.hpp>
#ifdef WIN32
# pragma comment(lib, "winmm.lib")
# include <boost/thread/locks.hpp>
# include <boost/thread/mutex.hpp>
# include <windows.h>
#else
# include <ctime>
# include <sys/time.h>
#endif

namespace beam {

class qtime {
 public:
  static const int UNIT_RATE_SEC = 1000000;
  static const int UNIT_RATE_MSEC = 1000;
  typedef int64_t unit_t;  // microsec

 private:
  unit_t t_;

  friend qtime get_qtime();

 public:
  qtime() {}
  qtime(const qtime& qt) : t_(qt.t_) {}
  explicit qtime(unit_t t) : t_(t) {}
  qtime& operator=(const qtime& qt) {
    t_ = qt.t_;
    return *this;
  }

  qtime operator-(qtime qt) const {
    return qtime(t_ - qt.t_);
  }

  operator int() const {
    return msec();
  }

  int msec() const {
    return t_ / UNIT_RATE_MSEC;
  }

  int64_t microsec() const {
    return t_;
  }
};

inline qtime get_qtime() {
#ifdef WIN32
  return qtime(timeGetTime() * qtime::UNIT_RATE_MSEC);
#else
  timeval tv;
  gettimeofday(&tv, 0);
  return qtime(tv.tv_sec * qtime::UNIT_RATE_SEC + tv.tv_usec);
#endif
}

class time_begin_period {
#ifdef WIN32
  class inner {
    boost::mutex m_;
    unsigned int p_;

   public:
    time_begin_period() : p_(-1) {}
    explicit time_begin_period(unsigned int p) : p_(p) {
      boost::lock_guard<boost::mutex> lock(m_);
      if (p != -1) timeBeginPeriod(p);
    }
    ~time_begin_period() {
      boost::lock_guard<boost::mutex> lock(m_);
      if (p_ != -1) timeEndPeriod(p);
    }

    void reset(unsigned int p = -1) {
      boost::lock_guard<boost::mutex> lock(m_);
      if (p_ != -1) timeEndPeriod(p_);
      p_ = p;
      if (p != -1) timeBeginPeriod(p);
    }
  };
#endif

 public:
  static void set(unsigned int p = 1) {
#ifdef WIN32
    singleton<inner>::get().reset(p);
#endif
  }
};

}  // namespace beam
