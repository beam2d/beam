#pragma once

// see:
// http://ja.wikibooks.org/wiki/
// More_C%2B%2B_Idioms/%E5%AE%89%E5%85%A8%E3%81%AA_bool(Safe_bool)

namespace beam {

struct safe_bool_base {
  typedef void (safe_bool_base::*bool_type)() const;
  void true_fn() const {}
};

template <typename T>
struct safe_bool : private safe_bool_base {
  operator bool_type() const {
    return static_cast<const T*>(this)->bool_test() ?
        &safe_bool_base::true_fn : 0;
  }
};

}  // namespace beam
