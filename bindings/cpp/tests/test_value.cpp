#include <cmath>
#include <limits>
#include <map>
#include <string>
#include <vector>

extern "C" {
#include <copenpublish.h>
}

#include <gtest/gtest.h>

#include "openpublish/status.h"
#include "openpublish/value.h"
#include "openpublish/value_iter.h"

#include "test_utils.h"

TEST(Value, NewNoneSuccess) {
  CPPOPENPUBLISH_NAMESPACE::Value value;
  CPPOPENPUBLISH_NAMESPACE::Status status;

  ASSERT_EQ(value.type(status), CPPOPENPUBLISH_NAMESPACE::Value::Type::None);
  validate_status_ok(status);
}

class BoolParameterizedTestFixture : public ::testing::TestWithParam<bool> {};

TEST_P(BoolParameterizedTestFixture, NewBoolSuccess) {
  bool v = GetParam();
  CPPOPENPUBLISH_NAMESPACE::Value value(v);
  CPPOPENPUBLISH_NAMESPACE::Status status;

  EXPECT_EQ(value.type(status), CPPOPENPUBLISH_NAMESPACE::Value::Type::Boolean);
  validate_status_ok(status);
  bool result = value.value_bool(status);
  validate_status_ok(status);

  EXPECT_EQ(result, v);
}

INSTANTIATE_TEST_SUITE_P(Value, BoolParameterizedTestFixture,
                         ::testing::Bool());

class IntParameterizedTestFixture : public ::testing::TestWithParam<int64_t> {};

TEST_P(IntParameterizedTestFixture, NewIntSuccess) {
  int64_t v = GetParam();
  CPPOPENPUBLISH_NAMESPACE::Value value(v);
  CPPOPENPUBLISH_NAMESPACE::Status status;

  EXPECT_EQ(value.type(status), CPPOPENPUBLISH_NAMESPACE::Value::Type::Integer);
  validate_status_ok(status);
  int64_t result = value.value_int(status);
  validate_status_ok(status);

  EXPECT_EQ(result, v);
}

INSTANTIATE_TEST_SUITE_P(
    Value, IntParameterizedTestFixture,
    ::testing::Values(std::numeric_limits<int64_t>::max(), 10, 1, 0, -1, -10,
                      std::numeric_limits<int64_t>::min()));

class FloatParameterizedTestFixture : public ::testing::TestWithParam<double> {
};

TEST_P(FloatParameterizedTestFixture, NewFloatSuccess) {
  double v = GetParam();
  CPPOPENPUBLISH_NAMESPACE::Value value(v);
  CPPOPENPUBLISH_NAMESPACE::Status status;

  EXPECT_EQ(value.type(status), CPPOPENPUBLISH_NAMESPACE::Value::Type::Float);
  validate_status_ok(status);
  double result = value.value_float(status);
  validate_status_ok(status);

  if (std::isnan(v)) {
    EXPECT_TRUE(std::isnan(result));
  } else {
    EXPECT_DOUBLE_EQ(result, v);
  }
}

INSTANTIATE_TEST_SUITE_P(
    Value, FloatParameterizedTestFixture,
    ::testing::Values(std::numeric_limits<double>::infinity(),
                      std::numeric_limits<double>::quiet_NaN(),
                      std::numeric_limits<double>::signaling_NaN(),
                      std::numeric_limits<double>::max(), 10.0, 1.0, 0.0, -1.0,
                      -10.0, std::numeric_limits<double>::min()));

class StringParameterizedTestFixture
    : public ::testing::TestWithParam<std::string> {};

TEST_P(StringParameterizedTestFixture, NewStringSuccess) {
  std::string v = GetParam();
  CPPOPENPUBLISH_NAMESPACE::Value value(v);
  CPPOPENPUBLISH_NAMESPACE::Status status;

  EXPECT_EQ(value.type(status), CPPOPENPUBLISH_NAMESPACE::Value::Type::String);
  validate_status_ok(status);
  std::string result = value.value_string(status);
  validate_status_ok(status);

  EXPECT_EQ(result, v);
}

INSTANTIATE_TEST_SUITE_P(Value, StringParameterizedTestFixture,
                         ::testing::Values("", "test", "1234"));

class ArrayParameterizedTestFixture
    : public ::testing::TestWithParam<
          std::vector<CPPOPENPUBLISH_NAMESPACE::Value>> {};

TEST_P(ArrayParameterizedTestFixture, NewArraySuccess) {
  std::vector<CPPOPENPUBLISH_NAMESPACE::Value> v = GetParam();
  CPPOPENPUBLISH_NAMESPACE::Status status;

  CPPOPENPUBLISH_NAMESPACE::Value value(v, status);
  validate_status_ok(status);

  EXPECT_EQ(value.type(status), CPPOPENPUBLISH_NAMESPACE::Value::Type::Array);
  validate_status_ok(status);

  CPPOPENPUBLISH_NAMESPACE::ValueIterArray iter =
      value.value_iter_array(status);
  validate_status_ok(status);

  size_t array_len = value.value_array_len(status);
  validate_status_ok(status);

  if (array_len == 1) {
    CPPOPENPUBLISH_NAMESPACE::ValueView v = value.value_array_get(0, status);
    ASSERT_EQ(v.type(status), CPPOPENPUBLISH_NAMESPACE::Value::Type::None);
    validate_status_ok(status);
  } else {
    size_t i = 0;

    for (; !iter.is_done(status); iter.next(status)) {
      validate_status_ok(status);
      CPPOPENPUBLISH_NAMESPACE::ValueView v = iter.value(status);
      validate_status_ok(status);

      int64_t result = v.value_int(status);
      validate_status_ok(status);

      ASSERT_EQ(result, i);
      i++;
    }

    ASSERT_EQ(i, array_len);
  }
}

TEST_P(ArrayParameterizedTestFixture, PushArraySuccess) {
  std::vector<CPPOPENPUBLISH_NAMESPACE::Value> v = GetParam();
  CPPOPENPUBLISH_NAMESPACE::Status status;

  CPPOPENPUBLISH_NAMESPACE::Value value(
      std::vector<CPPOPENPUBLISH_NAMESPACE::Value>{}, status);
  validate_status_ok(status);

  EXPECT_EQ(value.type(status), CPPOPENPUBLISH_NAMESPACE::Value::Type::Array);
  validate_status_ok(status);

  for (auto &&i : v) {
    value.value_array_push(i, status);
    validate_status_ok(status);
  }

  CPPOPENPUBLISH_NAMESPACE::ValueIterArray iter =
      value.value_iter_array(status);
  validate_status_ok(status);

  size_t array_len = value.value_array_len(status);
  validate_status_ok(status);

  if (array_len == 1) {
    CPPOPENPUBLISH_NAMESPACE::ValueView v = value.value_array_get(0, status);
    ASSERT_EQ(v.type(status), CPPOPENPUBLISH_NAMESPACE::Value::Type::None);
    validate_status_ok(status);
  } else {
    size_t i = 0;

    for (; !iter.is_done(status); iter.next(status)) {
      validate_status_ok(status);
      CPPOPENPUBLISH_NAMESPACE::ValueView v = iter.value(status);
      validate_status_ok(status);

      int64_t result = v.value_int(status);
      validate_status_ok(status);

      ASSERT_EQ(result, i);
      i++;
    }

    ASSERT_EQ(i, array_len);
  }
}

INSTANTIATE_TEST_SUITE_P(
    Value, ArrayParameterizedTestFixture,
    ::testing::Values(std::vector<CPPOPENPUBLISH_NAMESPACE::Value>{},
                      std::vector<CPPOPENPUBLISH_NAMESPACE::Value>{
                          CPPOPENPUBLISH_NAMESPACE::Value()},
                      std::vector<CPPOPENPUBLISH_NAMESPACE::Value>{
                          CPPOPENPUBLISH_NAMESPACE::Value((int64_t)0),
                          CPPOPENPUBLISH_NAMESPACE::Value((int64_t)1),
                          CPPOPENPUBLISH_NAMESPACE::Value((int64_t)2)}));

class ObjectParameterizedTestFixture
    : public ::testing::TestWithParam<
          std::map<std::string, CPPOPENPUBLISH_NAMESPACE::Value>> {};

TEST_P(ObjectParameterizedTestFixture, NewObjectSuccess) {
  std::map<std::string, CPPOPENPUBLISH_NAMESPACE::Value> v = GetParam();
  CPPOPENPUBLISH_NAMESPACE::Status status;

  CPPOPENPUBLISH_NAMESPACE::Value value(v, status);
  validate_status_ok(status);

  EXPECT_EQ(value.type(status), CPPOPENPUBLISH_NAMESPACE::Value::Type::Object);
  validate_status_ok(status);

  CPPOPENPUBLISH_NAMESPACE::ValueIterObject iter =
      value.value_iter_object(status);
  validate_status_ok(status);

  size_t object_len = value.value_object_len(status);
  validate_status_ok(status);

  if (object_len == 1) {
    CPPOPENPUBLISH_NAMESPACE::ValueView v =
        value.value_object_get("test", status);
    EXPECT_EQ(v.type(status), CPPOPENPUBLISH_NAMESPACE::Value::Type::None);
    validate_status_ok(status);
  } else {
    size_t i = 0;

    for (; !iter.is_done(status); iter.next(status)) {
      validate_status_ok(status);
      std::string_view k = iter.key(status);
      validate_status_ok(status);
      CPPOPENPUBLISH_NAMESPACE::ValueView v = iter.value(status);
      validate_status_ok(status);

      int64_t result = v.value_int(status);
      validate_status_ok(status);

      ASSERT_EQ(k, std::to_string(result));
      i++;
    }

    ASSERT_EQ(i, object_len);
  }
}

TEST_P(ObjectParameterizedTestFixture, InsertObjectSuccess) {
  std::map<std::string, CPPOPENPUBLISH_NAMESPACE::Value> v = GetParam();
  CPPOPENPUBLISH_NAMESPACE::Status status;

  CPPOPENPUBLISH_NAMESPACE::Value value(
      std::map<std::string, CPPOPENPUBLISH_NAMESPACE::Value>{}, status);
  validate_status_ok(status);

  for (auto &&i : v) {
    value.value_object_insert(i.first, i.second, status);
    validate_status_ok(status);
  }

  EXPECT_EQ(value.type(status), CPPOPENPUBLISH_NAMESPACE::Value::Type::Object);
  validate_status_ok(status);

  CPPOPENPUBLISH_NAMESPACE::ValueIterObject iter =
      value.value_iter_object(status);
  validate_status_ok(status);

  size_t object_len = value.value_object_len(status);
  validate_status_ok(status);

  if (object_len == 1) {
    CPPOPENPUBLISH_NAMESPACE::ValueView v =
        value.value_object_get("test", status);
    EXPECT_EQ(v.type(status), CPPOPENPUBLISH_NAMESPACE::Value::Type::None);
    validate_status_ok(status);
  } else {
    size_t i = 0;

    for (; !iter.is_done(status); iter.next(status)) {
      validate_status_ok(status);
      std::string_view k = iter.key(status);
      validate_status_ok(status);
      CPPOPENPUBLISH_NAMESPACE::ValueView v = iter.value(status);
      validate_status_ok(status);

      int64_t result = v.value_int(status);
      validate_status_ok(status);

      ASSERT_EQ(k, std::to_string(result));
      i++;
    }

    ASSERT_EQ(i, object_len);
  }
}

INSTANTIATE_TEST_SUITE_P(
    Value, ObjectParameterizedTestFixture,
    ::testing::Values(std::map<std::string, CPPOPENPUBLISH_NAMESPACE::Value>{},
                      std::map<std::string, CPPOPENPUBLISH_NAMESPACE::Value>{
                          {"test", CPPOPENPUBLISH_NAMESPACE::Value()}},
                      std::map<std::string, CPPOPENPUBLISH_NAMESPACE::Value>{
                          {"0", CPPOPENPUBLISH_NAMESPACE::Value((int64_t)0)},
                          {"1", CPPOPENPUBLISH_NAMESPACE::Value((int64_t)1)},
                          {"2", CPPOPENPUBLISH_NAMESPACE::Value((int64_t)2)}}));
