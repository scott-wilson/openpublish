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
#include "cpppublish/value_iter.h"

namespace CPPPUBLISH_NAMESPACE {
class ValueIterArray;
class ValueIterObject;
class Value;

class ValueView {
public:
  enum Type {
    None = CPublishValueTypeNone,
    Boolean = CPublishValueTypeBoolean,
    Integer = CPublishValueTypeInteger,
    Float = CPublishValueTypeFloat,
    String = CPublishValueTypeString,
    Array = CPublishValueTypeArray,
    Object = CPublishValueTypeObject,
  };

  ValueView(const CPublishValue *c_value);
  ValueView(const Value &value);
  ~ValueView();

  bool operator==(const ValueView &other) const;

  bool operator==(const Value &other) const;

  bool operator==(const bool other) const;

  bool operator==(const int64_t other) const;

  bool operator==(const double other) const;

  bool operator==(const std::string other) const;

  bool operator==(const std::vector<CPPPUBLISH_NAMESPACE::Value> other) const;

  bool operator==(
      const std::map<std::string, CPPPUBLISH_NAMESPACE::Value> other) const;

  const ValueView::Type type(CPPPUBLISH_NAMESPACE::Status &status) const;

  bool value_bool(CPPPUBLISH_NAMESPACE::Status &status) const;

  int64_t value_int(CPPPUBLISH_NAMESPACE::Status &status) const;

  double value_float(CPPPUBLISH_NAMESPACE::Status &status) const;

  std::string value_string(CPPPUBLISH_NAMESPACE::Status &status) const;

  size_t value_array_len(CPPPUBLISH_NAMESPACE::Status &status) const;

  const ValueView value_array_get(size_t index,
                                  CPPPUBLISH_NAMESPACE::Status &status) const;

  ValueIterArray value_iter_array(CPPPUBLISH_NAMESPACE::Status &status) const;

  size_t value_object_len(CPPPUBLISH_NAMESPACE::Status &status) const;

  const ValueView value_object_get(std::string &key,
                                   CPPPUBLISH_NAMESPACE::Status &status) const;

  ValueIterObject value_iter_object(CPPPUBLISH_NAMESPACE::Status &status) const;

  const CPublishValue *c_ptr() const;

private:
  const CPublishValue *_c_value;
};
} // namespace CPPPUBLISH_NAMESPACE
