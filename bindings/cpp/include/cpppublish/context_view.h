#pragma once

extern "C" {
#include <cpublish.h>
}

#include <string>

#include "cpppublish/context.h"
#include "cpppublish/status.h"
#include "cpppublish/value.h"

namespace CPPPUBLISH_NAMESPACE {
class Context;

class ContextView {
public:
  ContextView(const CPublishContext *context);
  ContextView(const CPPPUBLISH_NAMESPACE::Context &context);
  ~ContextView();

  const CPPPUBLISH_NAMESPACE::ValueView
  get(std::string &key, CPPPUBLISH_NAMESPACE::Status &status);

  size_t len(CPPPUBLISH_NAMESPACE::Status &status) const;

  bool is_empty(CPPPUBLISH_NAMESPACE::Status &status) const;

  Context clone(CPPPUBLISH_NAMESPACE::Status &status) const;

  const CPublishContext *c_ptr() const;

private:
  const CPublishContext *_c_context;
};

} // namespace CPPPUBLISH_NAMESPACE
