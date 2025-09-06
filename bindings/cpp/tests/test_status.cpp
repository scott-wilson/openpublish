#include <string>

extern "C" {
#include <copenpublish.h>
}

#include <gtest/gtest.h>

#include "openpublish/status.h"

#include "test_utils.h"

TEST(Status, NewSuccess) {
  CPPOPENPUBLISH_NAMESPACE::Status status;
  ASSERT_EQ(status.is_ok(), true);
  ASSERT_EQ(status.is_error(), false);
}

TEST(Status, CopySuccess) {
  CPPOPENPUBLISH_NAMESPACE::Status status;
  CPPOPENPUBLISH_NAMESPACE::Status other_status(status);
  status.set_ok();
  other_status.set_error("test");

  ASSERT_EQ(status.is_ok(), true);
  ASSERT_EQ(other_status.is_error(), true);
}

TEST(Status, CWrapSuccess) {
  OpenPublishStatus c_status;
  openpublish_status_ok(&c_status);
  CPPOPENPUBLISH_NAMESPACE::Status status(&c_status);
  openpublish_status_error(&c_status, "test");

  ASSERT_EQ(status.is_ok(), true);

  CPPOPENPUBLISH_NAMESPACE::Status other_status(&c_status);
  ASSERT_EQ(other_status.is_error(), true);
  ASSERT_EQ(other_status.message(), "test");

  openpublish_status_destroy(&c_status);
}

TEST(Status, SetOkSuccess) {
  CPPOPENPUBLISH_NAMESPACE::Status status;
  status.set_ok();
  ASSERT_EQ(status.is_ok(), true);
  ASSERT_EQ(status.is_error(), false);
}

TEST(Status, SetErrorSuccess) {
  CPPOPENPUBLISH_NAMESPACE::Status status;
  status.set_error("test");
  ASSERT_EQ(status.is_ok(), false);
  ASSERT_EQ(status.is_error(), true);
  ASSERT_EQ(status.message(), "test");

  CPPOPENPUBLISH_NAMESPACE::Status other_status;
  other_status.set_error(std::string("test"));
  ASSERT_EQ(other_status.is_ok(), false);
  ASSERT_EQ(other_status.is_error(), true);
  ASSERT_EQ(other_status.message(), "test");
}
