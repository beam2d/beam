#pragma once

#include <algorithm>
#include <utility>
#include "tr1/type_traits.h"

namespace beam {

// Struct for size compression of type T.
// If T is empty, then class inheriting cstruct<T> has empty size for instance
// of T.
template <typename T, bool IsEmpty = std::tr1::is_empty<T>::value >
class cstruct {
  T t_;

 public:
  static const bool is_empty = false;
  typedef T value_type;

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
template <typename T> class cstruct<T, true> : T {
 public:
  static const bool is_empty = true;
  typedef T value_type;

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


template <typename First, typename Second>
struct cpair : private cstruct<First>, private cstruct<Second> {
  typedef First first_type;
  typedef Second second_type;

  cpair() {}
  cpair(const cpair& c)
      : cstruct<First>(*static_cast<const cstruct<First>*>(&c)),
        cstruct<Second>(*static_cast<const cstruct<Second>*>(&c)) {}
  cpair(const First& f, const Second& s)
      : cstruct<First>(f), cstruct<Second>(s) {}

  template <typename U, typename V>
  cpair(const cpair<U, V>& c)
      : cstruct<First>(*static_cast<const U*>(&c)),
        cstruct<Second>(*static_cast<const V*>(&c)) {}

  template <typename U, typename V>
  cpair(const std::pair<U, V>& p)
      : cstruct<First>(p.first), cstruct<Second>(p.second) {}

  First& first() {
    return static_cast<cstruct<First>*>(this)->get();
  }
  const First& first() const {
    return static_cast<const cstruct<First>*>(this)->get();
  }

  Second& second() {
    return static_cast<cstruct<Second>*>(this)->get();
  }
  const Second& second() const {
    return static_cast<const cstruct<Second>*>(this)->get();
  }

  void swap(cpair& cp) {
    static_cast<cstruct<First>*>(this)->swap(cp);
    static_cast<cstruct<Second>*>(this)->swap(cp);
  }
};

template <typename First, typename Second>
inline void swap(cpair<First, Second>& a, cpair<First, Second>& b) {
  a.swap(b);
}

}  // namespace beam
