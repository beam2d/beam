#pragma once

#include <cstddef>
#include "preprocessor.h"

namespace beam {

template <size_t> struct static_assert_test {};
template <bool> struct static_assert_failure;
template <> struct static_assert_failure<true> {};

}  // namespace beam

# define BEAM_STATIC_ASSERT(cond) \
  typedef ::beam::static_assert_test<sizeof(static_assert_failure<!!(cond)>)> \
  BEAM_CAT(beam_static_assert_typedef_at_, BEAM_LINE)

#if !defined(BEAM_NO_SHORT_MACRO) && !defined(BEAM_NO_SHORT_STATIC_ASSERT)
# define STATIC_ASSERT BEAM_STATIC_ASSERT
#endif
