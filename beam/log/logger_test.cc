#include <iostream>
#include <sstream>
#include <string>
#include <gtest/gtest.h>
#include "../singleton.h"
#include "logger.h"
#include "logging_server.h"
using namespace std;

namespace beam {
namespace {

class logger_test : public ::testing::Test {
  ostringstream o_;

 protected:
  virtual void SetUp() {
    reset();
    singleton<log::logging_server>::get().stream(o_);
  }

  virtual void TearDown() {
    singleton<log::logging_server>::get().stream(cerr);
  }

  void reset() {
    o_.str("");
  }

  string str() const {
    return o_.str();
  }
};

TEST_F(logger_test, general_usage) {
  LOG(INFO) << "message";
  string msg = str();
  EXPECT_EQ(0, msg.find("[INFO]"));
  EXPECT_NE(string::npos, msg.find("message"));
}

TEST_F(logger_test, level) {
  LOG(INFO) << "message";
  EXPECT_FALSE(str().empty());

  reset();
  LOG(WARN) << "message";
  EXPECT_FALSE(str().empty());

  reset();
  LOG(ERROR) << "message";
  EXPECT_FALSE(str().empty());

  reset();
  EXPECT_THROW(LOG(FATAL) << "message", log::fatal_log);
  EXPECT_FALSE(str().empty());

  log::logging_server& ls = singleton<log::logging_server>::get();
  ls.level(log::LOGLEVEL_ERROR);

  reset();
  LOG(INFO) << "message";
  EXPECT_TRUE(str().empty());

  reset();
  LOG(WARN) << "message";
  EXPECT_TRUE(str().empty());

  reset();
  LOG(ERROR) << "message";
  EXPECT_FALSE(str().empty());
}

TEST_F(logger_test, check) {
  EXPECT_NO_THROW(CHECK(1) << "message");
  EXPECT_THROW(CHECK(0) << "message", log::check_error);
  EXPECT_FALSE(str().empty());
  EXPECT_EQ(0, str().find("[CHECK]"));
}

class some_module {};
class another_module {};

TEST_F(logger_test, vlog) {
  log::logging_server& ls = singleton<log::logging_server>::get();
  ls.verbosity<some_module>(1);

  VLOG(some_module, 0) << "message";
  EXPECT_FALSE(str().empty());
  EXPECT_EQ(0, str().find("[some_module(0)]"));
  EXPECT_NE(string::npos, str().find("message"));

  reset();
  VLOG(some_module, 1) << "message";
  EXPECT_FALSE(str().empty());

  reset();
  VLOG(some_module, 2) << "message";
  EXPECT_TRUE(str().empty());

  reset();
  VLOG(some_module, 3) << "message";
  EXPECT_TRUE(str().empty());

  reset();
  ls.verbosity<another_module>(3);
  VLOG(another_module, 3) << "message";
  EXPECT_FALSE(str().empty());
}

}  // anonymous namespace
}  // namespace beam
