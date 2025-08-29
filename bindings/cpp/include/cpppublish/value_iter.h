#pragma once

extern "C" {
#include <cpublish.h>
}

#include <iterator>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "cpppublish/status.h"
#include "cpppublish/value.h"

namespace CPPPUBLISH_NAMESPACE {
class Value;
class ValueView;

class ValueIterArray {
public:
  ValueIterArray(const CPPPUBLISH_NAMESPACE::Value &value,
                 CPPPUBLISH_NAMESPACE::Status &status);

  ValueIterArray(const CPPPUBLISH_NAMESPACE::ValueView &value,
                 CPPPUBLISH_NAMESPACE::Status &status);

  ~ValueIterArray();

  bool is_done(CPPPUBLISH_NAMESPACE::Status &status) const;

  void next(CPPPUBLISH_NAMESPACE::Status &status);

  const ValueView value(CPPPUBLISH_NAMESPACE::Status &status) const;

private:
  CPublishValueIterArray *_iter;
};

class ValueIterObject {
public:
  ValueIterObject(const CPPPUBLISH_NAMESPACE::Value &value,
                  CPPPUBLISH_NAMESPACE::Status &status);

  ValueIterObject(const CPPPUBLISH_NAMESPACE::ValueView &value,
                  CPPPUBLISH_NAMESPACE::Status &status);

  ~ValueIterObject();

  bool is_done(CPPPUBLISH_NAMESPACE::Status &status) const;

  void next(CPPPUBLISH_NAMESPACE::Status &status);

  const std::string_view key(CPPPUBLISH_NAMESPACE::Status &status) const;
  const ValueView value(CPPPUBLISH_NAMESPACE::Status &status) const;

private:
  CPublishValueIterObject *_iter;
};
} // namespace CPPPUBLISH_NAMESPACE
