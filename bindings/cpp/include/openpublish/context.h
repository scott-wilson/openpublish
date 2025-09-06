#pragma once

extern "C" {
#include <copenpublish.h>
}

#include <map>
#include <string>
#include <vector>

#include "openpublish/context_iter.h"
#include "openpublish/status.h"
#include "openpublish/value.h"
#include "openpublish/value_view.h"

namespace CPPOPENPUBLISH_NAMESPACE {
class ContextIter;

class Context {
public:
  Context();
  Context(const Context &other);
  Context(OpenPublishContext *c_context);
  ~Context();

  const CPPOPENPUBLISH_NAMESPACE::ValueView
  get(const std::string &key, CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  void set(const std::string &key, const CPPOPENPUBLISH_NAMESPACE::Value &value,
           CPPOPENPUBLISH_NAMESPACE::Status &status);

  void set(const std::string &key, CPPOPENPUBLISH_NAMESPACE::Status &status);

  void set(const std::string &key, bool value,
           CPPOPENPUBLISH_NAMESPACE::Status &status);

  void set(const std::string &key, int64_t value,
           CPPOPENPUBLISH_NAMESPACE::Status &status);

  void set(const std::string &key, double value,
           CPPOPENPUBLISH_NAMESPACE::Status &status);

  void set(const std::string &key, const std::string &value,
           CPPOPENPUBLISH_NAMESPACE::Status &status);

  void set(const std::string &key,
           const std::vector<CPPOPENPUBLISH_NAMESPACE::Value> &value,
           CPPOPENPUBLISH_NAMESPACE::Status &status);

  void set(const std::string &key,
           const std::map<std::string, CPPOPENPUBLISH_NAMESPACE::Value> &value,
           CPPOPENPUBLISH_NAMESPACE::Status &status);

  size_t len(CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  bool is_empty(CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  Context clone(CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  const OpenPublishContext *c_ptr() const;

  OpenPublishContext *c_ptr_mut() const;

private:
  OpenPublishContext *_c_context;
};
} // namespace CPPOPENPUBLISH_NAMESPACE
