#pragma once

namespace beam {

// Convinient type/value to avoid ODR when we want to define a class with
// static member variables without *.cc files.
enum avoid_odr {
  AVOID_ODR = 0
};

}  // namespace beam
