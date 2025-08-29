#include <cmath>
#include <limits>
#include <map>
#include <string>
#include <vector>

extern "C" {
#include <cpublish.h>
}

#include <gtest/gtest.h>

#include "cpppublish/context.h"
#include "cpppublish/context_iter.h"
#include "cpppublish/status.h"
#include "cpppublish/value.h"
#include "cpppublish/value_iter.h"

#include "test_utils.h"

class ContextValueParameterizedTestFixture
    : public ::testing::TestWithParam<
          std::map<std::string, CPPPUBLISH_NAMESPACE::Value>> {};

TEST_P(ContextValueParameterizedTestFixture, InsertSuccess) {
  std::map<std::string, CPPPUBLISH_NAMESPACE::Value> items = GetParam();
  CPPPUBLISH_NAMESPACE::Status status;
  CPPPUBLISH_NAMESPACE::Context context;
  size_t expected_len = 0;

  for (auto &&item : items) {
    context.set(item.first, item.second, status);
    validate_status_ok(status);
    expected_len++;
  }

  size_t result_len = 0;

  for (auto &&item : items) {
    CPPPUBLISH_NAMESPACE::ValueView value = context.get(item.first, status);
    validate_status_ok(status);

    auto input_type = item.second.type(status);
    validate_status_ok(status);
    auto result_type = value.type(status);
    validate_status_ok(status);

    EXPECT_EQ(input_type, result_type);
    validate_status_ok(status);

    auto input_value = item.second.value_int(status);
    validate_status_ok(status);
    auto result_value = value.value_int(status);
    validate_status_ok(status);

    EXPECT_EQ(input_value, result_value);
    validate_status_ok(status);

    result_len++;
  }

  EXPECT_EQ(result_len, expected_len);

  size_t len = context.len(status);
  validate_status_ok(status);

  EXPECT_EQ(len, expected_len);
}

INSTANTIATE_TEST_SUITE_P(
    Value, ContextValueParameterizedTestFixture,
    ::testing::Values(std::map<std::string, CPPPUBLISH_NAMESPACE::Value>{},
                      std::map<std::string, CPPPUBLISH_NAMESPACE::Value>{
                          {"0", CPPPUBLISH_NAMESPACE::Value{(int64_t)0}}}));

TEST(Context, NewNoneValueSuccess) {
  CPPPUBLISH_NAMESPACE::Status status;
  CPPPUBLISH_NAMESPACE::Context context;
  context.set("0", status);
  validate_status_ok(status);
  size_t len = context.len(status);
  validate_status_ok(status);
  EXPECT_EQ(len, 1);

  CPPPUBLISH_NAMESPACE::ValueView value = context.get("0", status);
  validate_status_ok(status);
  EXPECT_EQ(value.type(status), CPPPUBLISH_NAMESPACE::Value::None);
  validate_status_ok(status);
}

class ContextBoolValueParameterizedTestFixture
    : public ::testing::TestWithParam<std::map<std::string, bool>> {};

TEST_P(ContextBoolValueParameterizedTestFixture, InsertSuccess) {
  std::map<std::string, bool> items = GetParam();
  CPPPUBLISH_NAMESPACE::Status status;
  CPPPUBLISH_NAMESPACE::Context context;
  size_t expected_len = 0;

  for (auto &&item : items) {
    context.set(item.first, item.second, status);
    validate_status_ok(status);
    expected_len++;
  }

  size_t result_len = 0;

  for (auto &&item : items) {
    CPPPUBLISH_NAMESPACE::ValueView value = context.get(item.first, status);
    validate_status_ok(status);

    auto result_type = value.type(status);
    validate_status_ok(status);

    EXPECT_EQ(result_type, CPPPUBLISH_NAMESPACE::Value::Boolean);

    auto input_value = item.second;
    auto result_value = value.value_bool(status);
    validate_status_ok(status);

    EXPECT_EQ(input_value, result_value);
    validate_status_ok(status);

    result_len++;
  }

  EXPECT_EQ(result_len, expected_len);

  size_t len = context.len(status);
  validate_status_ok(status);

  EXPECT_EQ(len, expected_len);
}

INSTANTIATE_TEST_SUITE_P(
    Value, ContextBoolValueParameterizedTestFixture,
    ::testing::Values(std::map<std::string, bool>{},
                      std::map<std::string, bool>{{"true", true}},
                      std::map<std::string, bool>{{"false", false}},
                      std::map<std::string, bool>{{"true", true},
                                                  {"false", false}}));

class ContextIntValueParameterizedTestFixture
    : public ::testing::TestWithParam<std::map<std::string, int64_t>> {};

TEST_P(ContextIntValueParameterizedTestFixture, InsertSuccess) {
  std::map<std::string, int64_t> items = GetParam();
  CPPPUBLISH_NAMESPACE::Status status;
  CPPPUBLISH_NAMESPACE::Context context;
  size_t expected_len = 0;

  for (auto &&item : items) {
    context.set(item.first, item.second, status);
    validate_status_ok(status);
    expected_len++;
  }

  size_t result_len = 0;

  for (auto &&item : items) {
    CPPPUBLISH_NAMESPACE::ValueView value = context.get(item.first, status);
    validate_status_ok(status);

    auto result_type = value.type(status);
    validate_status_ok(status);

    EXPECT_EQ(result_type, CPPPUBLISH_NAMESPACE::Value::Integer);

    auto input_value = item.second;
    auto result_value = value.value_int(status);
    validate_status_ok(status);

    EXPECT_EQ(input_value, result_value);

    result_len++;
  }

  EXPECT_EQ(result_len, expected_len);

  size_t len = context.len(status);
  validate_status_ok(status);

  EXPECT_EQ(len, expected_len);
}

INSTANTIATE_TEST_SUITE_P(
    Value, ContextIntValueParameterizedTestFixture,
    ::testing::Values(std::map<std::string, int64_t>{},
                      std::map<std::string, int64_t>{{"0", 0}},
                      std::map<std::string, int64_t>{{"1", 1}},
                      std::map<std::string, int64_t>{{"10", 10}},
                      std::map<std::string, int64_t>{{"-10", -10}},
                      std::map<std::string, int64_t>{{"0", 0}, {"1", 1}}));

class ContextFloatValueParameterizedTestFixture
    : public ::testing::TestWithParam<std::map<std::string, double>> {};

TEST_P(ContextFloatValueParameterizedTestFixture, InsertSuccess) {
  std::map<std::string, double> items = GetParam();
  CPPPUBLISH_NAMESPACE::Status status;
  CPPPUBLISH_NAMESPACE::Context context;
  size_t expected_len = 0;

  for (auto &&item : items) {
    context.set(item.first, item.second, status);
    validate_status_ok(status);
    expected_len++;
  }

  size_t result_len = 0;

  for (auto &&item : items) {
    CPPPUBLISH_NAMESPACE::ValueView value = context.get(item.first, status);
    validate_status_ok(status);

    auto result_type = value.type(status);
    validate_status_ok(status);

    EXPECT_EQ(result_type, CPPPUBLISH_NAMESPACE::Value::Float);

    auto input_value = item.second;
    auto result_value = value.value_float(status);
    validate_status_ok(status);

    EXPECT_EQ(input_value, result_value);

    result_len++;
  }

  EXPECT_EQ(result_len, expected_len);

  size_t len = context.len(status);
  validate_status_ok(status);

  EXPECT_EQ(len, expected_len);
}

INSTANTIATE_TEST_SUITE_P(
    Value, ContextFloatValueParameterizedTestFixture,
    ::testing::Values(std::map<std::string, double>{},
                      std::map<std::string, double>{{"0.0", 0.0}},
                      std::map<std::string, double>{{"1.0", 1.0}},
                      std::map<std::string, double>{{"10.0", 10.0}},
                      std::map<std::string, double>{{"-10.0", -10.0}},
                      std::map<std::string, double>{{"0.0", 0.0},
                                                    {"1.0", 1.0}}));

class ContextStringValueParameterizedTestFixture
    : public ::testing::TestWithParam<std::map<std::string, std::string>> {};

TEST_P(ContextStringValueParameterizedTestFixture, InsertSuccess) {
  std::map<std::string, std::string> items = GetParam();
  CPPPUBLISH_NAMESPACE::Status status;
  CPPPUBLISH_NAMESPACE::Context context;
  size_t expected_len = 0;

  for (auto &&item : items) {
    context.set(item.first, item.second, status);
    validate_status_ok(status);
    expected_len++;
  }

  size_t result_len = 0;

  for (auto &&item : items) {
    CPPPUBLISH_NAMESPACE::ValueView value = context.get(item.first, status);
    validate_status_ok(status);

    auto result_type = value.type(status);
    validate_status_ok(status);

    EXPECT_EQ(result_type, CPPPUBLISH_NAMESPACE::Value::String);

    auto input_value = item.second;
    auto result_value = value.value_string(status);
    validate_status_ok(status);

    EXPECT_EQ(input_value, result_value);

    result_len++;
  }

  EXPECT_EQ(result_len, expected_len);

  size_t len = context.len(status);
  validate_status_ok(status);

  EXPECT_EQ(len, expected_len);
}

INSTANTIATE_TEST_SUITE_P(
    Value, ContextStringValueParameterizedTestFixture,
    ::testing::Values(std::map<std::string, std::string>{},
                      std::map<std::string, std::string>{{"", ""}},
                      std::map<std::string, std::string>{{"test", "test"}},
                      std::map<std::string, std::string>{{"", ""},
                                                         {"test", "test"}}));

class ContextArrayValueParameterizedTestFixture
    : public ::testing::TestWithParam<
          std::map<std::string, std::vector<CPPPUBLISH_NAMESPACE::Value>>> {};

TEST_P(ContextArrayValueParameterizedTestFixture, InsertSuccess) {
  std::map<std::string, std::vector<CPPPUBLISH_NAMESPACE::Value>> items =
      GetParam();
  CPPPUBLISH_NAMESPACE::Status status;
  CPPPUBLISH_NAMESPACE::Context context;
  size_t expected_len = 0;

  for (auto &&item : items) {
    context.set(item.first, item.second, status);
    validate_status_ok(status);
    expected_len++;
  }

  size_t result_len = 0;

  for (auto &&item : items) {
    CPPPUBLISH_NAMESPACE::ValueView value = context.get(item.first, status);
    validate_status_ok(status);

    auto result_type = value.type(status);
    validate_status_ok(status);

    EXPECT_EQ(result_type, CPPPUBLISH_NAMESPACE::Value::Array);

    auto input_value = item.second;

    ASSERT_EQ(item.second.size(), value.value_array_len(status));
    validate_status_ok(status);

    result_len++;
  }

  EXPECT_EQ(result_len, expected_len);

  size_t len = context.len(status);
  validate_status_ok(status);

  EXPECT_EQ(len, expected_len);
}

INSTANTIATE_TEST_SUITE_P(
    Value, ContextArrayValueParameterizedTestFixture,
    ::testing::Values(
        std::map<std::string, std::vector<CPPPUBLISH_NAMESPACE::Value>>{},
        std::map<std::string, std::vector<CPPPUBLISH_NAMESPACE::Value>>{
            {"", std::vector{CPPPUBLISH_NAMESPACE::Value()}}}));

class ContextObjectValueParameterizedTestFixture
    : public ::testing::TestWithParam<std::map<
          std::string, std::map<std::string, CPPPUBLISH_NAMESPACE::Value>>> {};

TEST_P(ContextObjectValueParameterizedTestFixture, InsertSuccess) {
  std::map<std::string, std::map<std::string, CPPPUBLISH_NAMESPACE::Value>>
      items = GetParam();
  CPPPUBLISH_NAMESPACE::Status status;
  CPPPUBLISH_NAMESPACE::Context context;
  size_t expected_len = 0;

  for (auto &&item : items) {
    context.set(item.first, item.second, status);
    validate_status_ok(status);
    expected_len++;
  }

  size_t result_len = 0;

  for (auto &&item : items) {
    CPPPUBLISH_NAMESPACE::ValueView value = context.get(item.first, status);
    validate_status_ok(status);

    auto result_type = value.type(status);
    validate_status_ok(status);

    EXPECT_EQ(result_type, CPPPUBLISH_NAMESPACE::Value::Object);

    auto input_value = item.second;

    ASSERT_EQ(item.second.size(), value.value_object_len(status));
    validate_status_ok(status);

    result_len++;
  }

  EXPECT_EQ(result_len, expected_len);

  size_t len = context.len(status);
  validate_status_ok(status);

  EXPECT_EQ(len, expected_len);
}

INSTANTIATE_TEST_SUITE_P(
    Value, ContextObjectValueParameterizedTestFixture,
    ::testing::Values(
        std::map<std::string,
                 std::map<std::string, CPPPUBLISH_NAMESPACE::Value>>{},
        std::map<std::string,
                 std::map<std::string, CPPPUBLISH_NAMESPACE::Value>>{
            {"", std::map<std::string, CPPPUBLISH_NAMESPACE::Value>{
                     {"test", CPPPUBLISH_NAMESPACE::Value()}}}}));
