#pragma once

namespace beam {

namespace noncopyable_avoid_adl {

class noncopyable {
  noncopyable(const noncopyable&);
  const noncopyable& operator=(const noncopyable&);
 protected:
  noncopyable() {}
};

}  // namespace beam::lang::noncopyable_avoid_adl

using noncopyable_avoid_adl::noncopyable;

}  // namespace beam
