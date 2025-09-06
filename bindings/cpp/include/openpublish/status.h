#pragma once

extern "C" {
#include <copenpublish.h>
}

#include <string>
#include <string_view>

namespace CPPOPENPUBLISH_NAMESPACE {
class Status : private OpenPublishStatus {
public:
  Status();

  Status(const OpenPublishStatus &status);

  Status(const OpenPublishStatus *status);

  ~Status();

  bool is_ok() const;

  bool is_error() const;

  const std::string_view message() const;

  void set_ok();

  void set_error(const std::string &message);
  void set_error(const char *message);
};

} // namespace CPPOPENPUBLISH_NAMESPACE
