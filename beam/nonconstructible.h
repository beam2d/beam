#pragma once

#include <boost/noncopyable.hpp>

namespace beam {

namespace nonconstructible_avoid_adl {

class nonconstructible : boost::noncopyable {
  nonconstructible();
  ~nonconstructible();
};

}  // namespace beam::nonconstructible_avoid_adl

using nonconstructible_avoid_adl::nonconstructible;

}  // namespace beam
