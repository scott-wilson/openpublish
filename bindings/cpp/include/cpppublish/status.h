#pragma once

extern "C" {
#include <cpublish.h>
}

#include <string>
#include <string_view>

namespace CPPPUBLISH_NAMESPACE {
class Status : private CPublishStatus {
public:
  Status();

  Status(const CPublishStatus &status);

  Status(const CPublishStatus *status);

  ~Status();

  bool is_ok() const;

  bool is_error() const;

  const std::string_view message() const;

  void set_ok();

  void set_error(const std::string &message);
  void set_error(const char *message);
};

} // namespace CPPPUBLISH_NAMESPACE
