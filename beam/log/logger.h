#pragma once

#include <exception>
#include <sstream>

// ERROR conflicts with symbol defined in windows.h
#ifdef WIN32
# include <windows.h>
# undef ERROR
#endif

#include "../noncopyable.h"
#include "../singleton.h"
#include "logging_server.h"

namespace beam {
namespace log {

namespace detail {

// General purpose logger
class logger : noncopyable {
  std::ostringstream oss_;
  log_level level_;

 public:
  explicit logger(log_level l) : level_(l) {}

  void write() const {
    logging_server& ls = singleton<logging_server>::get();
    ls.write(level_, oss_.str());
  }

  template <typename T> logger& operator<<(const T& t) {
    oss_ << t;
    return *this;
  }
};

// Module-wise logger
template <typename Module> class verbose_logger : noncopyable {
  std::ostringstream oss_;
  int verbosity_;

 public:
  explicit verbose_logger(int v) : verbosity_(v) {}

  void write() const {
    logging_server& ls = singleton<logging_server>::get();
    ls.verbose_write<Module>(verbosity_, oss_.str());
  }

  template <typename T> verbose_logger& operator<<(const T& t) {
    oss_ << t;
    return *this;
  }
};

// Exception type which is thrown on (D)CHECK failure
class check_error : virtual std::exception {};

// Logger for (D)CHECK
class exception_logger : noncopyable {
  std::ostringstream oss_;
  log_level level_;

 public:
  exception_logger() : level_(LOGLEVEL_FATAL) {}
  explicit exception_logger(log_level l) : level_(l) {}

  void write() const {
    logging_server& ls = singleton<logging_server>::get();
    ls.write(level_, oss_.str());
    throw check_error();
  }

  template <typename T> exception_logger& operator<<(const T& t) {
    oss_ << t;
    return *this;
  }
};

struct logger_write_trigger {
  template <typename T> void operator&(const T& t) const {
    t.write();
  }
};

template <log_level L> struct logger_of_level {
  typedef logger type;
};
template <> struct logger_of_level<LOGLEVEL_FATAL> {
  typedef exception_logger type;
};

struct null_stream {
  struct void_op {
    void operator&(null_stream) {}
  };
  template <typename T> null_stream& operator<<(T) const {
    return *this;
  }
};
}  // namespace beam::log::detail
}  // namespace beam::log
}  // namespace beam


#define BEAM_LOG_LEVELVALUE_(level) ::beam::log::LOGLEVEL_ ## level

#ifndef BEAM_LOG_PREFIX
# define BEAM_LOG_STRINGIZE_(x) #x
# define BEAM_LOG_PREFIX(type)                                    \
  "[" #type "] " __FILE__ "(" BEAM_LOG_STRINGIZE_(__LINE__) "): "
#endif

#define BEAM_LOG(level)                                                 \
  ::beam::log::detail::logger_write_trigger() &                         \
  (::beam::log::detail::logger_of_level<BEAM_LOG_LEVELVALUE_(level)>::  \
   type(BEAM_LOG_LEVELVALUE_(level)))                                   \
  << BEAM_LOG_PREFIX(#level)

#define BEAM_VLOG(module, verbosity)                        \
  ::beam::log::detail::logger_write_trigger() &             \
  (::beam::log::detail::verbose_logger<module>(verbosity))  \
  << BEAM_LOG_PREFIX(#module "(" #verbosity ")")

#define BEAM_CHECK(cond) \
  (cond) ? (void)0 : ::beam::log::detail::logger_write_trigger() & \
  (::beam::log::detail::exception_logger()) << BEAM_LOG_PREFIX(CHECK)

#ifdef NDEBUG
# define BEAM_LOG_NULL_STREAM_                                 \
  1 ? (void)0 : ::beam::log::detail::null_stream::void_op() &  \
  ::beam::log::detail::null_stream()
# define BEAM_DLOG(...) BEAM_LOG_NULL_STREAM_
# define BEAM_DCHECK(...) BEAM_LOG_NULL_STREAM_
#else
# define BEAM_DLOG BEAM_LOG
# define BEAM_DCHECK BEAM_CHECK
#endif  // _DEBUG

#if !defined(BEAM_NO_SHORT_MACRO) && !defined(BEAM_NO_SHORT_LOG_MACRO)
# define LOG BEAM_LOG
# define VLOG BEAM_VLOG
# define CHECK BEAM_CHECK
# define DLOG BEAM_DLOG
# define DCHECK BEAM_DCHECK
#endif
