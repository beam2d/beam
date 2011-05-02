#include <algorithm>
#include <gtest/gtest.h>
#include "../beam/checked_delete.h"
#include "../beam/scoped_ptr.h"

namespace beam {
namespace {

class set_on_destruction {
  bool& flag_;

 public:
  explicit set_on_destruction(bool& flag) : flag_(flag) {}
  ~set_on_destruction() { flag_ = true; }
};

struct struct_with_member {
  int val;
};

class sample_class {
  bool& flag_;

 public:
  explicit sample_class(bool& flag) : flag_(flag) {}
  void set() { flag_ = true; }
};

struct custom_deleter {
  void operator()(sample_class* p) const {
    p->set();
    checked_delete(p);
  }
};


TEST(scoped_ptr_test, arrow) {
  struct_with_member* p = new struct_with_member;
  p->val = 10;
  scoped_ptr<struct_with_member> sp(p);
  EXPECT_EQ(p->val, sp->val);
}

TEST(scoped_ptr_test, custom_deleter) {
  bool flag = false;
  {
    scoped_ptr<sample_class, custom_deleter> sp(new sample_class(flag));
  }
  EXPECT_TRUE(flag);
}

TEST(scoped_ptr_test, dereference) {
  int* p = new int(10);
  scoped_ptr<int> sp(p);
  EXPECT_EQ(*p, *sp);
}

TEST(scoped_ptr_test, get) {
  int* p = new int;
  scoped_ptr<int> sp(p);
  EXPECT_EQ(p, sp.get());
}

TEST(scoped_ptr_test, reset) {
  bool deleted1 = false, deleted2 = false;
  set_on_destruction* p1 = new set_on_destruction(deleted1);
  set_on_destruction* p2 = new set_on_destruction(deleted2);
  scoped_ptr<set_on_destruction> sp(p1);
  sp.reset(p2);
  EXPECT_EQ(p2, sp.get());
  EXPECT_TRUE(deleted1);
  EXPECT_FALSE(deleted2);
}

TEST(scoped_ptr_test, reset_null) {
  bool deleted = false;
  set_on_destruction* p = new set_on_destruction(deleted);
  scoped_ptr<set_on_destruction> sp(p);
  sp.reset();
  EXPECT_EQ(0, sp.get());
  EXPECT_TRUE(deleted);
}

TEST(scoped_ptr_test, safe_bool) {
  scoped_ptr<int> sp;
  EXPECT_FALSE(sp);

  sp.reset(new int);
  EXPECT_TRUE(sp);
}

TEST(scoped_ptr_test, scope) {
  bool flag = false;
  {
    scoped_ptr<set_on_destruction> sp(new set_on_destruction(flag));
    EXPECT_FALSE(flag);
  }
  EXPECT_TRUE(flag);
}

TEST(scoped_ptr_test, swap) {
  int* p1 = new int(1);
  int* p2 = new int(2);
  scoped_ptr<int> sp1(p1), sp2(p2);
  sp1.swap(sp2);
  EXPECT_EQ(p1, sp2.get());
  EXPECT_EQ(p2, sp1.get());

  swap(sp1, sp2);
  EXPECT_EQ(p1, sp1.get());
  EXPECT_EQ(p2, sp2.get());
}

}  // anonymous namespace
}  // namespace beam
