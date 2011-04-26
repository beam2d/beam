#pragma once

#include "noncopyable.h"

namespace beam {

namespace nonconstructible_avoid_adl {

class nonconstructible : noncopyable {
  nonconstructible();
  ~nonconstructible();
};

}  // namespace beam::nonconstructible_avoid_adl

using nonconstructible_avoid_adl::nonconstructible;

}  // namespace beam
