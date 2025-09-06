#pragma once

extern "C" {
#include <copenpublish.h>
}

#include <string>

#include "openpublish/context.h"
#include "openpublish/status.h"
#include "openpublish/value.h"

namespace CPPOPENPUBLISH_NAMESPACE {
class Context;

class ContextView {
public:
  ContextView(const OpenPublishContext *context);
  ContextView(const CPPOPENPUBLISH_NAMESPACE::Context &context);
  ~ContextView();

  const CPPOPENPUBLISH_NAMESPACE::ValueView
  get(std::string &key, CPPOPENPUBLISH_NAMESPACE::Status &status);

  size_t len(CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  bool is_empty(CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  Context clone(CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  const OpenPublishContext *c_ptr() const;

private:
  const OpenPublishContext *_c_context;
};

} // namespace CPPOPENPUBLISH_NAMESPACE
