#pragma once

extern "C" {
#include <cpublish.h>
}

#include <map>
#include <string>
#include <vector>

#include "cpppublish/context_iter.h"
#include "cpppublish/status.h"
#include "cpppublish/value.h"
#include "cpppublish/value_view.h"

namespace CPPPUBLISH_NAMESPACE {
class ContextIter;

class Context {
public:
  Context();
  Context(const Context &other);
  Context(CPublishContext *c_context);
  ~Context();

  const CPPPUBLISH_NAMESPACE::ValueView
  get(const std::string &key, CPPPUBLISH_NAMESPACE::Status &status) const;

  void set(const std::string &key, const CPPPUBLISH_NAMESPACE::Value &value,
           CPPPUBLISH_NAMESPACE::Status &status);

  void set(const std::string &key, CPPPUBLISH_NAMESPACE::Status &status);

  void set(const std::string &key, bool value,
           CPPPUBLISH_NAMESPACE::Status &status);

  void set(const std::string &key, int64_t value,
           CPPPUBLISH_NAMESPACE::Status &status);

  void set(const std::string &key, double value,
           CPPPUBLISH_NAMESPACE::Status &status);

  void set(const std::string &key, const std::string &value,
           CPPPUBLISH_NAMESPACE::Status &status);

  void set(const std::string &key,
           const std::vector<CPPPUBLISH_NAMESPACE::Value> &value,
           CPPPUBLISH_NAMESPACE::Status &status);

  void set(const std::string &key,
           const std::map<std::string, CPPPUBLISH_NAMESPACE::Value> &value,
           CPPPUBLISH_NAMESPACE::Status &status);

  size_t len(CPPPUBLISH_NAMESPACE::Status &status) const;

  bool is_empty(CPPPUBLISH_NAMESPACE::Status &status) const;

  Context clone(CPPPUBLISH_NAMESPACE::Status &status) const;

  const CPublishContext *c_ptr() const;

  CPublishContext *c_ptr_mut() const;

private:
  CPublishContext *_c_context;
};
} // namespace CPPPUBLISH_NAMESPACE
