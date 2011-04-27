#pragma once

#ifndef BEAM_NO_RTTI
# include <typeinfo>
#endif


namespace beam {

#ifndef BEAM_NO_RTTI

typedef const char* type_id;

template <typename T> inline type_id get_type_id() {
  return typeid(T).name();
}

#else

typedef char* type_id;

template <typename T> struct type_id_holder {
  static char place;
  static type_id get_id() {
    return &place;
  }
};

template <typename T> char type_id_holder<T>::place;

template <typename T> inline type_id get_type_id() {
  return type_id_holder<T>::get_id();
}

#endif

}  // namespace beam
