#pragma once

#include <algorithm>
#include <cassert>
#include "checked_delete.h"
#include "cpair.h"
#include "noncopyable.h"
#include "safe_bool.h"

namespace beam {

// Deleter must not throw.
template <typename T, typename Deleter = checked_deleter<T> >
class scoped_ptr : public safe_bool<scoped_ptr<T> >, noncopyable {
  cpair<T*, Deleter> cp_;

 public:
  explicit scoped_ptr(T* p = 0) : cp_(p, Deleter()) {}  // never throws
  scoped_ptr(T* p, Deleter d) : cp_(p, d) {}  // never throws

  ~scoped_ptr() {  // never throws
    cp_.second()(cp_.first());
  }

  // safe_bool
  bool bool_test() const {  // never throws
    return cp_.first();
  }

  T* get() const {  // never throws
    return cp_.first();
  }

  void reset(T* p = 0) {  // never throws
    assert(p == 0 || p != cp_.first());
    scoped_ptr(p).swap(*this);
  }

  void swap(scoped_ptr& sp) {  // never throws
    cp_.swap(sp.cp_);
  }

  T& operator*() const {  // never throws
    return *cp_.first();
  }

  T* operator->() const {  // never throws
    return cp_.first();
  }
};

template <typename T>
inline void swap(scoped_ptr<T>& lhs, scoped_ptr<T>& rhs) {  // never throws
  lhs.swap(rhs);
}


// Deleter must not throw.
template <typename T, typename Deleter = checked_array_deleter<T> >
class scoped_array : public scoped_ptr<T, Deleter> {
 public:
  scoped_array() {}  // never throws
  // never throws
  explicit scoped_array(T* p = 0) : scoped_ptr<T, Deleter>(p) {}
  // never throws
  scoped_array(T* p, Deleter d) : scoped_ptr<T, Deleter>(p, d) {}

  ~scoped_array() {  // never throws
    ~scoped_ptr<T, Deleter>();
  }

  T& operator[](size_t i) const {  // never throws
    return this->get()[i];
  }
};

}  // namespace beam
