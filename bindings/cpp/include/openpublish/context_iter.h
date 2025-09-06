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

class ContextIter {
public:
  ContextIter(const CPPOPENPUBLISH_NAMESPACE::Context &context,
              CPPOPENPUBLISH_NAMESPACE::Status &status);
  ~ContextIter();

  bool is_done(CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  void next(CPPOPENPUBLISH_NAMESPACE::Status &status);

  const ValueView value(CPPOPENPUBLISH_NAMESPACE::Status &status) const;

private:
  OpenPublishContextIter *_iter;
};
} // namespace CPPOPENPUBLISH_NAMESPACE
