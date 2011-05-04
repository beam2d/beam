#pragma once

#include <algorithm>
#include <cassert>
#include <boost/assert.hpp>
#include <boost/checked_delete.hpp>
#include <boost/compressed_pair.hpp>
#include <boost/noncopyable.hpp>
#include "safe_bool.h"

namespace beam {

// Deleter must not throw.
template <typename T, typename Deleter = boost::checked_deleter<T> >
class scoped_ptr
    : public safe_bool<scoped_ptr<T, Deleter> >, boost::noncopyable {
  boost::compressed_pair<T*, Deleter> cp_;

 public:
  explicit scoped_ptr(T* p = 0) : cp_(p, Deleter()) {}
  scoped_ptr(T* p, Deleter d) : cp_(p, d) {}

  ~scoped_ptr() {
    cp_.second()(cp_.first());
  }

  // safe_bool
  bool bool_test() const {
    return cp_.first();
  }

  T* get() const {
    return cp_.first();
  }

  void reset(T* p = 0) {
    BOOST_ASSERT(p == 0 || p != cp_.first());
    scoped_ptr(p).swap(*this);
  }

  void swap(scoped_ptr& sp) {
    cp_.swap(sp.cp_);
  }

  T& operator*() const {
    return *cp_.first();
  }

  T* operator->() const {
    return cp_.first();
  }
};

template <typename T>
inline void swap(scoped_ptr<T>& lhs, scoped_ptr<T>& rhs) {
  lhs.swap(rhs);
}


// Deleter must not throw.
template <typename T, typename Deleter = boost::checked_array_deleter<T> >
class scoped_array : public scoped_ptr<T, Deleter> {
 public:
  scoped_array() {}

  explicit scoped_array(T* p = 0) : scoped_ptr<T, Deleter>(p) {}

  scoped_array(T* p, Deleter d) : scoped_ptr<T, Deleter>(p, d) {}

  ~scoped_array() {
    ~scoped_ptr<T, Deleter>();
  }

  T& operator[](size_t i) const {
    return this->get()[i];
  }
};

}  // namespace beam
