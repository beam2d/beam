#include <gtest/gtest.h>
#include "../beam/scoped_ptr.h"

namespace {

class set_on_destruction {
  bool& flag_;

 public:
  explicit set_on_destruction(bool& flag) : flag_(flag) {}
  ~set_on_destruction() { flag_ = true; }
};

TEST(scoped_ptr_test, scope) {
  bool flag = false;
  {
    beam::scoped_ptr<set_on_destruction> sp(new set_on_destruction(flag));
    EXPECT_FALSE(flag);
  }
  EXPECT_TRUE(flag);
}

}  // anonymous namespace
