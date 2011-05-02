#include <iostream>
#include <utility>
#include <gtest/gtest.h>
#include "cpair.h"
#include "static_assert.h"

namespace beam {
namespace {

STATIC_ASSERT(sizeof(cpair<int, int>) == sizeof(std::pair<int, int>));
STATIC_ASSERT(
    sizeof(cpair<long long, int>) == sizeof(std::pair<long long, int>));

struct empty_struct {};
struct empty_function {
  void operator()() const {}
};

STATIC_ASSERT(sizeof(cpair<int, empty_struct>) == sizeof(int));
STATIC_ASSERT(sizeof(cpair<empty_struct, int>) == sizeof(int));
STATIC_ASSERT(sizeof(cpair<int, empty_function>) == sizeof(int));
STATIC_ASSERT(sizeof(cpair<empty_function, int>) == sizeof(int));
STATIC_ASSERT(
    sizeof(cpair<empty_struct, empty_function>) == sizeof(empty_struct));
STATIC_ASSERT(
    sizeof(cpair<empty_function, empty_struct>) == sizeof(empty_struct));
STATIC_ASSERT(
    sizeof(cpair<empty_struct, empty_struct>) == sizeof(empty_struct));
STATIC_ASSERT(
    sizeof(cpair<empty_function, empty_function>) == sizeof(empty_struct));

}  // anonymous namespace
}  // namespace beam
