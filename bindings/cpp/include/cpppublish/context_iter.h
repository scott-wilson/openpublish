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

class ContextIter {
public:
  ContextIter(const CPPPUBLISH_NAMESPACE::Context &context,
              CPPPUBLISH_NAMESPACE::Status &status);
  ~ContextIter();

  bool is_done(CPPPUBLISH_NAMESPACE::Status &status) const;

  void next(CPPPUBLISH_NAMESPACE::Status &status);

  const ValueView value(CPPPUBLISH_NAMESPACE::Status &status) const;

private:
  CPublishContextIter *_iter;
};
} // namespace CPPPUBLISH_NAMESPACE
