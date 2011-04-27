#pragma once

#include <iostream>
#include <string>
#include "../mutex.h"
#include "../tr1/unordered_map.h"
#include "../type_id.h"

namespace beam {
namespace log {

enum log_level {
  LOGLEVEL_INFO = 0,
  LOGLEVEL_WARN,
  LOGLEVEL_ERROR,
  LOGLEVEL_FATAL
};

class logging_server {
  std::tr1::unordered_map<type_id, int> module_verbosities_;
  mutex mutex_;
  std::ostream* stream_;
  log_level level_;

 public:
  logging_server() : stream_(&std::cerr), level_(LOGLEVEL_INFO) {}

  void level(log_level l) {
    level_ = l;
  }

  void stream(std::ostream& s) {
    stream_ = &s;
  }

  template <typename Module> void verbosity(int verbosity) {
    module_verbosities_[get_type_id<Module>()] = verbosity;
  }

  void write(log_level l, const std::string& message) {
    if (l < level_) return;
    scoped_lock<mutex> lock(mutex_);
    (*stream_) << message << std::endl;
  }

  template <typename Module>
  void verbose_write(int verbosity, const std::string& message) {
    if (module_verbosities_[get_type_id<Module>()] < verbosity) return;
    scoped_lock<mutex> lock(mutex_);
    (*stream_) << message << std::endl;
  }
};

}  // namespace beam::log
}  // namespace beam
