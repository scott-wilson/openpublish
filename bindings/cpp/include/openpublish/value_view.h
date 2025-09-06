#pragma once

extern "C" {
#include <copenpublish.h>
}

#include <iterator>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "openpublish/status.h"
#include "openpublish/value.h"
#include "openpublish/value_iter.h"

namespace CPPOPENPUBLISH_NAMESPACE {
class ValueIterArray;
class ValueIterObject;
class Value;

class ValueView {
public:
  enum Type {
    None = OpenPublishValueTypeNone,
    Boolean = OpenPublishValueTypeBoolean,
    Integer = OpenPublishValueTypeInteger,
    Float = OpenPublishValueTypeFloat,
    String = OpenPublishValueTypeString,
    Array = OpenPublishValueTypeArray,
    Object = OpenPublishValueTypeObject,
  };

  ValueView(const OpenPublishValue *c_value);
  ValueView(const Value &value);
  ~ValueView();

  bool operator==(const ValueView &other) const;

  bool operator==(const Value &other) const;

  bool operator==(const bool other) const;

  bool operator==(const int64_t other) const;

  bool operator==(const double other) const;

  bool operator==(const std::string other) const;

  bool operator==(const std::vector<CPPOPENPUBLISH_NAMESPACE::Value> other) const;

  bool operator==(
      const std::map<std::string, CPPOPENPUBLISH_NAMESPACE::Value> other) const;

  const ValueView::Type type(CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  bool value_bool(CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  int64_t value_int(CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  double value_float(CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  std::string value_string(CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  size_t value_array_len(CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  const ValueView value_array_get(size_t index,
                                  CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  ValueIterArray value_iter_array(CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  size_t value_object_len(CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  const ValueView value_object_get(std::string &key,
                                   CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  ValueIterObject value_iter_object(CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  const OpenPublishValue *c_ptr() const;

private:
  const OpenPublishValue *_c_value;
};
} // namespace CPPOPENPUBLISH_NAMESPACE
