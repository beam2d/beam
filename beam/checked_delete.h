#pragma once

#include <functional>

namespace beam {

// NOTE: These functions must be called with beam:: even if their names are
// imported by using directive or using declarations, in order to avoid any
// unintended ADL effects.

template <typename T>
inline void force_complete_type() {
  typedef char error_if_the_type_is_incomplete[sizeof(T) ? 1 : -1];
  (void) sizeof(error_if_the_type_is_incomplete);
}

template <typename T>
inline void checked_delete(T* p) {
  beam::force_complete_type<T>();
  delete p;
}

template <typename T>
inline void checked_array_delete(T* p) {
  beam::force_complete_type<T>();
  delete [] p;
}

template <typename T>
struct checked_deleter : public std::unary_function<T*, void> {
  void operator()(T* p) const {
    beam::checked_delete(p);
  }
};

template <typename T>
struct checked_array_deleter : public std::unary_function<T*, void> {
  void operator()(T* p) const {
    beam::checked_array_delete(p);
  }
};

}  // namespace beam
