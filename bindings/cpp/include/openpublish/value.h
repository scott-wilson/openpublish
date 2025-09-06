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
#include "openpublish/value_iter.h"
#include "openpublish/value_view.h"

namespace CPPOPENPUBLISH_NAMESPACE {
class ValueIterArray;
class ValueIterObject;
class ValueView;

class Value {
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

  Value();

  Value(const Value &other);

  Value(bool value);

  Value(int64_t value);

  Value(double value);

  Value(const std::string &value);

  Value(const std::vector<Value> &value, CPPOPENPUBLISH_NAMESPACE::Status &status);

  Value(const std::map<std::string, Value> &value,
        CPPOPENPUBLISH_NAMESPACE::Status &status);

  ~Value();

  bool operator==(const Value &other) const;

  bool operator==(const ValueView &other) const;

  bool operator==(const bool other) const;

  bool operator==(const int64_t other) const;

  bool operator==(const double other) const;

  bool operator==(const std::string other) const;

  bool operator==(const std::vector<CPPOPENPUBLISH_NAMESPACE::Value> other) const;

  bool operator==(
      const std::map<std::string, CPPOPENPUBLISH_NAMESPACE::Value> other) const;

  const Value::Type type(CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  bool value_bool(CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  int64_t value_int(CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  double value_float(CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  std::string value_string(CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  size_t value_array_len(CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  const ValueView value_array_get(size_t index,
                                  CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  void value_array_push(const Value &value,
                        CPPOPENPUBLISH_NAMESPACE::Status &status);

  ValueIterArray value_iter_array(CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  size_t value_object_len(CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  const ValueView value_object_get(const std::string &key,
                                   CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  void value_object_insert(const std::string &key, const Value &value,
                           CPPOPENPUBLISH_NAMESPACE::Status &status);

  ValueIterObject value_iter_object(CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  const OpenPublishValue *c_ptr() const;

private:
  OpenPublishValue *_c_value;
};
} // namespace CPPOPENPUBLISH_NAMESPACE
