#pragma once

#include <algorithm>
#include <utility>
#include "tr1/type_traits.h"

namespace beam {

// Struct for size compression of type T.
// If T is empty, then class inheriting cstruct<T> has empty size for instance
// of T.
template <typename T, int = 0, bool IsEmpty = std::tr1::is_empty<T>::value >
class cstruct {
  T t_;

 public:
  cstruct() {}
  cstruct(const cstruct& c) : t_(c.t_) {}
  cstruct(const T& t) : t_(t) {}

  T& get() { return t_; }
  const T& get() const { return t_; }

  void swap(cstruct& c) {
    using std::swap;
    swap(t_, c.t_);
  }
};

// case that T is empty
template <typename T, int Index> class cstruct<T, Index, true> : T {
 public:
  cstruct() {}
  cstruct(const cstruct& c) : T(*static_cast<const T*>(c)) {}
  cstruct(const T& t) : T(t) {}

  T& get() { return *static_cast<T*>(this); }
  const T& get() const { return *static_cast<const T*>(this); }

  void swap(cstruct& c) {
    using std::swap;
    swap(get(), c.get());
  }
};

template <typename T, bool B>
inline void swap(cstruct<T, B>& a, cstruct<T, B>& b) {
  a.swap(b);
}


template <typename First,
          typename Second,
          bool SecondIsEmpty = std::tr1::is_empty<Second>::value>
struct cpair : private cstruct<First, 0>, private cstruct<Second, 1> {
  typedef First first_type;
  typedef Second second_type;
  typedef cstruct<First, 0> first_wrapper_type;
  typedef cstruct<Second, 1> second_wrapper_type;

  cpair() {}
  cpair(const cpair& c)
      : cstruct<First, 0>(static_cast<const cstruct<First, 0>*>(&c)->get()),
        cstruct<Second, 1>(static_cast<const cstruct<Second, 1>*>(&c)->get()) {
  }
  cpair(const First& f, const Second& s)
      : cstruct<First, 0>(f), cstruct<Second, 1>(s) {}

  template <typename U, typename V>
  cpair(const cpair<U, V>& c)
      : cstruct<First, 0>(
          static_cast<const typename cpair<U, V>::first_wrapper_type*>(&c)
          ->get()),
        cstruct<Second, 1>(
          static_cast<const typename cpair<U, V>::second_wrapper_type*>(&c)
          ->get())
  {}

  template <typename U, typename V>
  cpair(const std::pair<U, V>& p)
      : cstruct<First, 0>(p.first), cstruct<Second, 1>(p.second) {}

  First& first() {
    return static_cast<cstruct<First, 0>*>(this)->get();
  }
  const First& first() const {
    return static_cast<const cstruct<First, 0>*>(this)->get();
  }

  Second& second() {
    return static_cast<cstruct<Second, 1>*>(this)->get();
  }
  const Second& second() const {
    return static_cast<const cstruct<Second, 1>*>(this)->get();
  }

  void swap(cpair& cp) {
    static_cast<cstruct<First, 0>*>(this)->swap(cp);
    static_cast<cstruct<Second, 1>*>(this)->swap(cp);
  }
};

// If First and Second is same empty type, then cpair should not inherit it by
// multiple paths, because it increases the size of cpair.
// e.g.)
//   class empty {};
//   class A : cstruct<empty, 0> {};
//   sizeof(A);  // 1
//   class B : cstruct<empty, 0>, cstruct<empty, 1> {};
//   sizeof(B);  // 2
// With specialization bellow, sizeof cpair<empty, empty> is reduced to 1.
template <typename T>
struct cpair<T, T, true> : cstruct<T, 0> {
  typedef T first_type;
  typedef T second_type;
  typedef cstruct<T, 0> first_wrapper_type;
  typedef cstruct<T, 0> second_wrapper_type;

  cpair() {}
  cpair(const cpair& c)
      : cstruct<T, 0>(static_cast<const cstruct<T, 0>*>(&c)->get()) {}
  cpair(const T& f, const T& s) : cstruct<T, 0>(f) {}

  template <typename U, typename V>
  cpair(const cpair<U, V>& c)
      : cstruct<T, 0>(
          static_cast<const typename cpair<U, V>::first_wrapper_type*>(&c)
          ->get()) {}

  template <typename U, typename V>
  cpair(const std::pair<U, V>& p) : cstruct<T, 0>(p.first) {}

  T& first() {
    return static_cast<cstruct<T, 0>*>(this)->get();
  }
  const T& first() const {
    return static_cast<const cstruct<T, 0>*>(this)->get();
  }

  T& second() {
    return first();
  }
  const T& second() const {
    return first();
  }

  void swap(cpair& cp) {
    static_cast<cstruct<T, 0>*>(this)->swap(cp);
  }
};

template <typename First, typename Second>
inline void swap(cpair<First, Second>& a, cpair<First, Second>& b) {
  a.swap(b);
}

}  // namespace beam
