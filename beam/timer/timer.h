#pragma once

#include <functional>
#include "get_time.h"
#include "ttime.h"

namespace beam {

class timer {
  ttime since_;

 public:
  timer() : since_(get_time()) {}
  explicit timer(ttime tt) : since_(tt) {}
  timer(const timer& tm) : since_(tm.since_) {}
  timer& operator=(timer tm) {
    since_ = tm.since_;
    return *this;
  }

  ttime since() const { return since_; }

  ttime operator()() const {
    return get_time() - since_;
  }
};

}  // namespace beam
