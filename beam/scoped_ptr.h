#pragma once

#include <algorithm>
#include <cassert>
#include "checked_delete.h"
#include "noncopyable.h"
#include "safe_bool.h"

namespace beam {

// Deleter must not throw.
template <typename T, typename Deleter = checked_deleter<T> >
class scoped_ptr : public safe_bool<scoped_ptr<T> >, noncopyable {
  T* ptr_;

 public:
  explicit scoped_ptr(T* ptr = 0) : ptr_(ptr) {}  // never throws

  ~scoped_ptr() {  // never throws
    Deleter()(ptr_);
  }

  // safe_bool
  bool bool_test() const {  // never throws
    return ptr_;
  }

  T* get() const {  // never throws
    return ptr_;
  }

  void reset(T* p = 0) {  // never throws
    assert(p == 0 || p != ptr_);
    scoped_ptr(p).swap(*this);
  }

  void swap(scoped_ptr& sp) {  // never throws
    T* p = ptr_;
    ptr_ = sp.ptr_;
    sp.ptr_ = p;
  }

  T& operator*() const {  // never throws
    return *ptr_;
  }

  T* operator->() const {  // never throws
    return ptr_;
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
  // never throws
  explicit scoped_array(T* ptr = 0) : scoped_ptr<T, Deleter>(ptr) {}

  ~scoped_array() {  // never throws
    ~scoped_ptr<T, Deleter>();
  }

  T& operator[](size_t i) const {  // never throws
    return this->get()[i];
  }
};

}  // namespace beam
