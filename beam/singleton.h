#pragma once

#include <cstdlib>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/once.hpp>
#include "avoid_odr.h"
#include "nonconstructible.h"

namespace beam {

template <typename T> class singleton;

template <avoid_odr>
class singleton_finalizer_tmpl : nonconstructible {
  template <typename T> friend class singleton;

  static std::vector<void (*)()> finalizers_;
  static boost::mutex mutex_;
  static bool registered_;

  static void finalize() {
    {
      boost::lock_guard<boost::mutex> lock(mutex_);
      registered_ = false;
    }
    while (!finalizers_.empty()) {
      std::vector<void (*)()> finalizers;
      {
        boost::lock_guard<boost::mutex> lock(mutex_);
        finalizers.swap(finalizers_);
      }
      while (!finalizers.empty()) {
        finalizers.back()();
        finalizers.pop_back();
      }
    }
  }

  static void register_finalizer(void (*finalizer)()) {
    boost::lock_guard<boost::mutex> lock(mutex_);
    if (!registered_) {
      std::atexit(finalize);
      registered_ = true;
    }
    finalizers_.push_back(finalizer);
  }
};

template <avoid_odr N>
std::vector<void (*)()> singleton_finalizer_tmpl<N>::finalizers_;
template <avoid_odr N> boost::mutex singleton_finalizer_tmpl<N>::mutex_;
template <avoid_odr N> bool singleton_finalizer_tmpl<N>::registered_;

typedef singleton_finalizer_tmpl<AVOID_ODR> singleton_finalizer;

// The singleton class template can be used with CRTP and/or intrusive ways.
// e.g.)
//   class A : public singleton<A> { ... };
//   class B { ... };
// NOTE: See notes of singleton::get. In most cases, T::~T should not call
// singleton<T>::get, which causes stack overflow.
template <typename T> class singleton : boost::noncopyable {
  static boost::once_flag once_flag_;
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
    // We cannot use boost::checked_delete, because it requires users to
    // declare boost::checked_delete as a friend of T, if T::~T is private.
    typedef char T_must_be_complete[sizeof(T) ? 1 : -1];
    (void) sizeof(T_must_be_complete);
    delete p;
  }

 protected:
  singleton() {}

 public:
  static T& get() {
    boost::call_once(once_flag_, init());
    if (!instance_) {
      // This code is executed only by static variables' destructors which are
      // called after singleton finalizers and call singleton::get. During the
      // static variables' destruction, we suppose only main thread is running,
      // so we ignore thread safety here.
      // NOTE: Be careful to stack overflow. If instance_->~T always calls
      // singleton<T>::get, it tries to create a new instance of T, and
      // instance_->~T will be called at exit again.
      init()();
    }
    return *instance_;
  }
};

template <typename T>
boost::once_flag singleton<T>::once_flag_ = BOOST_ONCE_INIT;
template <typename T> T* singleton<T>::instance_ = 0;

}  // namespace beam
