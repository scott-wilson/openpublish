// #include <cmath>
// #include <limits>
// #include <string>
// #include <vector>
// #include <map>
#include <optional>
#include <string>
#include <utility>

// extern "C"
// {
// #include <copenpublish.h>
// }

#include <gtest/gtest.h>

#include "openpublish/context.h"
#include "openpublish/context_iter.h"
#include "openpublish/publish.h"
#include "openpublish/runner.h"
#include "openpublish/status.h"
#include "openpublish/value.h"
#include "openpublish/value_iter.h"

#include "test_utils.h"

class Publish : public CPPOPENPUBLISH_NAMESPACE::BasePublish {
public:
  Publish(std::optional<std::pair<std::string, bool>> pre_publish_context,
          std::optional<std::string> pre_publish_status,
          std::optional<std::string> rollback_pre_publish_status,
          std::optional<std::pair<std::string, bool>> publish_context,
          std::optional<std::string> publish_status,
          std::optional<std::string> rollback_publish_status,
          std::optional<std::pair<std::string, bool>> post_publish_context,
          std::optional<std::string> post_publish_status,
          std::optional<std::string> rollback_post_publish_status)
      : pre_publish_context(pre_publish_context),
        pre_publish_status(pre_publish_status),
        rollback_pre_publish_status(rollback_pre_publish_status),
        publish_context(publish_context), publish_status(publish_status),
        rollback_publish_status(rollback_publish_status),
        post_publish_context(post_publish_context),
        post_publish_status(post_publish_status),
        rollback_post_publish_status(rollback_post_publish_status) {}

  virtual std::optional<CPPOPENPUBLISH_NAMESPACE::Context>
  pre_publish(const CPPOPENPUBLISH_NAMESPACE::ContextView &context,
              CPPOPENPUBLISH_NAMESPACE::Status &status) {
    std::optional<CPPOPENPUBLISH_NAMESPACE::Context> ctx;

    if (pre_publish_context.has_value()) {
      ctx = context.clone(status);
      ctx.value().set(pre_publish_context.value().first,
                      pre_publish_context.value().second, status);
    } else {
      ctx = std::nullopt;
    }

    if (pre_publish_status.has_value()) {
      status.set_error(pre_publish_status.value());
    } else {
      status.set_ok();
    }

    return ctx;
  }

  virtual void
  rollback_pre_publish(const CPPOPENPUBLISH_NAMESPACE::ContextView &context,
                       CPPOPENPUBLISH_NAMESPACE::Status &status) {
    if (rollback_pre_publish_status.has_value()) {
      status.set_error(rollback_pre_publish_status.value());
    } else {
      status.set_ok();
    }
  }

  virtual std::optional<CPPOPENPUBLISH_NAMESPACE::Context>
  publish(const CPPOPENPUBLISH_NAMESPACE::ContextView &context,
          CPPOPENPUBLISH_NAMESPACE::Status &status) {
    std::optional<CPPOPENPUBLISH_NAMESPACE::Context> ctx;

    if (publish_context.has_value()) {
      ctx = context.clone(status);
      ctx.value().set(publish_context.value().first,
                      publish_context.value().second, status);
    } else {
      ctx = std::nullopt;
    }

    if (publish_status.has_value()) {
      status.set_error(publish_status.value());
    } else {
      status.set_ok();
    }

    return ctx;
  }

  virtual void
  rollback_publish(const CPPOPENPUBLISH_NAMESPACE::ContextView &context,
                   CPPOPENPUBLISH_NAMESPACE::Status &status) {
    if (rollback_publish_status.has_value()) {
      status.set_error(rollback_publish_status.value());
    } else {
      status.set_ok();
    }
  }

  virtual std::optional<CPPOPENPUBLISH_NAMESPACE::Context>
  post_publish(const CPPOPENPUBLISH_NAMESPACE::ContextView &context,
               CPPOPENPUBLISH_NAMESPACE::Status &status) {
    std::optional<CPPOPENPUBLISH_NAMESPACE::Context> ctx;

    if (post_publish_context.has_value()) {
      ctx = context.clone(status);
      ctx.value().set(post_publish_context.value().first,
                      post_publish_context.value().second, status);
    } else {
      ctx = std::nullopt;
    }

    if (post_publish_status.has_value()) {
      status.set_error(post_publish_status.value());
    } else {
      status.set_ok();
    }

    return ctx;
  }

  virtual void
  rollback_post_publish(const CPPOPENPUBLISH_NAMESPACE::ContextView &context,
                        CPPOPENPUBLISH_NAMESPACE::Status &status) {
    if (rollback_post_publish_status.has_value()) {
      status.set_error(rollback_post_publish_status.value());
    } else {
      status.set_ok();
    }
  }

  std::optional<std::pair<std::string, bool>> pre_publish_context;
  std::optional<std::string> pre_publish_status;
  std::optional<std::string> rollback_pre_publish_status;
  std::optional<std::pair<std::string, bool>> publish_context;
  std::optional<std::string> publish_status;
  std::optional<std::string> rollback_publish_status;
  std::optional<std::pair<std::string, bool>> post_publish_context;
  std::optional<std::string> post_publish_status;
  std::optional<std::string> rollback_post_publish_status;
};

class CheckParameterizedTestFixture
    : public ::testing::TestWithParam<std::tuple<
          std::optional<std::pair<std::string, bool>>, // pre_publish_context
          std::optional<std::string>,                  // pre_publish_status
          std::optional<std::string>, // rollback_pre_publish_status
          std::optional<std::pair<std::string, bool>>, // publish_context
          std::optional<std::string>,                  // publish_status
          std::optional<std::string>, // rollback_publish_status
          std::optional<std::pair<std::string, bool>>, // post_publish_context
          std::optional<std::string>,                  // post_publish_status
          std::optional<std::string> // rollback_post_publish_status
          >> {};

TEST_P(CheckParameterizedTestFixture, RunCheck) {
  std::optional<std::pair<std::string, bool>> pre_publish_context =
      std::get<0>(GetParam());
  std::optional<std::string> pre_publish_status = std::get<1>(GetParam());
  std::optional<std::string> rollback_pre_publish_status =
      std::get<2>(GetParam());
  std::optional<std::pair<std::string, bool>> publish_context =
      std::get<3>(GetParam());
  std::optional<std::string> publish_status = std::get<4>(GetParam());
  std::optional<std::string> rollback_publish_status = std::get<5>(GetParam());
  std::optional<std::pair<std::string, bool>> post_publish_context =
      std::get<6>(GetParam());
  std::optional<std::string> post_publish_status = std::get<7>(GetParam());
  std::optional<std::string> rollback_post_publish_status =
      std::get<8>(GetParam());
  bool should_fail =
      pre_publish_status.has_value() ||
      rollback_pre_publish_status.has_value() || publish_status.has_value() ||
      rollback_publish_status.has_value() || post_publish_status.has_value() ||
      rollback_post_publish_status.has_value();
  Publish publish = Publish{
      pre_publish_context,  pre_publish_status,  rollback_pre_publish_status,
      publish_context,      publish_status,      rollback_publish_status,
      post_publish_context, post_publish_status, rollback_post_publish_status};

  CPPOPENPUBLISH_NAMESPACE::Status status;
  CPPOPENPUBLISH_NAMESPACE::Context context =
      CPPOPENPUBLISH_NAMESPACE::run(publish, status);

  if (should_fail) {
    EXPECT_TRUE(status.is_error());
  } else {
    EXPECT_TRUE(status.is_ok());
  }

  if (pre_publish_context.has_value()) {
    EXPECT_EQ(context.get(pre_publish_context.value().first, status),
              CPPOPENPUBLISH_NAMESPACE::Value(pre_publish_context.value().second));
    EXPECT_TRUE(status.is_ok());
  }
  if (pre_publish_context.has_value()) {
    EXPECT_EQ(context.get(pre_publish_context.value().first, status),
              CPPOPENPUBLISH_NAMESPACE::Value(pre_publish_context.value().second));
    EXPECT_TRUE(status.is_ok());
  }
}

INSTANTIATE_TEST_SUITE_P(
    Value, CheckParameterizedTestFixture,
    ::testing::Combine(
        ::testing::Values(
            std::nullopt,
            std::pair<std::string, bool>{"pre_publish_context", true},
            std::pair<std::string, bool>{"pre_publish_context",
                                         false}), // pre_publish_context
        ::testing::Values(
            std::nullopt,
            std::string("failed pre_publish_context")), // pre_publish_status
        ::testing::Values(
            std::nullopt,
            std::string(
                "failed rollback_pre_publish_context")), // rollback_pre_publish_status
        ::testing::Values(std::nullopt,
                          std::pair<std::string, bool>{"publish_context", true},
                          std::pair<std::string, bool>{
                              "publish_context", false}), // publish_context
        ::testing::Values(
            std::nullopt,
            std::string("failed publish_context")), // publish_status
        ::testing::Values(
            std::nullopt,
            std::string(
                "failed rollback_publish_context")), // rollback_publish_status
        ::testing::Values(
            std::nullopt,
            std::pair<std::string, bool>{"post_publish_context", true},
            std::pair<std::string, bool>{"post_publish_context",
                                         false}), // post_publish_context
        ::testing::Values(
            std::nullopt,
            std::string("failed post_publish_context")), // post_publish_status
        ::testing::Values(
            std::nullopt,
            std::string(
                "failed rollback_post_publish_context")) // rollback_post_publish_status
        ));
