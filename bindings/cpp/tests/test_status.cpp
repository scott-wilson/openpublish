#include <string>

extern "C" {
#include <cpublish.h>
}

#include <gtest/gtest.h>

#include "cpppublish/status.h"

#include "test_utils.h"

TEST(Status, NewSuccess) {
  CPPPUBLISH_NAMESPACE::Status status;
  ASSERT_EQ(status.is_ok(), true);
  ASSERT_EQ(status.is_error(), false);
}

TEST(Status, CopySuccess) {
  CPPPUBLISH_NAMESPACE::Status status;
  CPPPUBLISH_NAMESPACE::Status other_status(status);
  status.set_ok();
  other_status.set_error("test");

  ASSERT_EQ(status.is_ok(), true);
  ASSERT_EQ(other_status.is_error(), true);
}

TEST(Status, CWrapSuccess) {
  CPublishStatus c_status;
  cpublish_status_ok(&c_status);
  CPPPUBLISH_NAMESPACE::Status status(&c_status);
  cpublish_status_error(&c_status, "test");

  ASSERT_EQ(status.is_ok(), true);

  CPPPUBLISH_NAMESPACE::Status other_status(&c_status);
  ASSERT_EQ(other_status.is_error(), true);
  ASSERT_EQ(other_status.message(), "test");

  cpublish_status_destroy(&c_status);
}

TEST(Status, SetOkSuccess) {
  CPPPUBLISH_NAMESPACE::Status status;
  status.set_ok();
  ASSERT_EQ(status.is_ok(), true);
  ASSERT_EQ(status.is_error(), false);
}

TEST(Status, SetErrorSuccess) {
  CPPPUBLISH_NAMESPACE::Status status;
  status.set_error("test");
  ASSERT_EQ(status.is_ok(), false);
  ASSERT_EQ(status.is_error(), true);
  ASSERT_EQ(status.message(), "test");

  CPPPUBLISH_NAMESPACE::Status other_status;
  other_status.set_error(std::string("test"));
  ASSERT_EQ(other_status.is_ok(), false);
  ASSERT_EQ(other_status.is_error(), true);
  ASSERT_EQ(other_status.message(), "test");
}
