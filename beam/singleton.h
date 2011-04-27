#pragma once

#include <cstdlib>
#include <vector>
#include "atomic.h"
#include "checked_delete.h"
#include "mutex.h"
#include "nonconstructible.h"

namespace beam {

template <typename T> class singleton;

template <bool N = false>
class singleton_finalizer_tmpl : nonconstructible {
  template <typename T> friend class singleton;

  static const int N_must_be_false = sizeof(int[N ? -1 : 1]);

  static mutex mutex_;
  static bool registered_;
  static std::vector<void (*)()> finalizers_;

  static void finalize() {
    {
      scoped_lock<mutex> lock(mutex_);
      registered_ = false;
    }
    while (!finalizers_.empty()) {
      std::vector<void (*)()> finalizers;
      {
        scoped_lock<mutex> lock(mutex_);
        finalizers.swap(finalizers_);
      }
      while (!finalizers.empty()) {
        finalizers.back()();
        finalizers.pop_back();
      }
    }
  }

  static void register_finalizer(void (*finalizer)()) {
    scoped_lock<mutex> lock(mutex_);
    if (!registered_) {
      std::atexit(finalize);
      registered_ = true;
    }
    finalizers_.push_back(finalizer);
  }
};

template <bool N> mutex singleton_finalizer_tmpl<N>::mutex_;
template <bool N> bool singleton_finalizer_tmpl<N>::registered_;
template <bool N>
std::vector<void (*)()> singleton_finalizer_tmpl<N>::finalizers_;

typedef singleton_finalizer_tmpl<> singleton_finalizer;

// The singleton class template can be used with CRTP and/or intrusive ways.
// e.g.)
//   class A : public singleton<A> { ... };
//   class B { ... };
// If A and/or B should be not constructable by others, they need to have
// singleton as a friend class.
// NOTE: See comments in singleton::get. For all T, T::~T must not call
// singleton<T>::get, which causes an infinite loop.
template <typename T> class singleton {
  static once_flag once_flag_;
  static T* instance_;

  struct init {
    void operator()() const {
      singleton_finalizer::register_finalizer(finalize);
      instance_ = new T;
    }
  };

  static void finalize() {
    T* p = instance_;
    instance_ = 0;
    checked_delete(instance_);
  }

 protected:
  singleton() {}

 public:
  static T& get() {
    call_once(once_flag_, init());
    if (!instance_) {
      // This code is executed only by static variables' destructors which are
      // called after singleton finalizers and call singleton::get. During the
      // static variables' destruction, we suppose only main thread is running,
      // so we ignore thread safety here.
      // NOTE: Be careful to an unintended infinite loop. If instance_->~T
      // calls singleton<T>::get, it tries to create new instance of T, and
      // instance_->~T will be called at exit again.
      init()();
    }
    return *instance_;
  }
};

template <typename T> once_flag singleton<T>::once_flag_ = ONCE_INIT;
template <typename T> T* singleton<T>::instance_;

}  // namespace beam
