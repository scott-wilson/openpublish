#include "cpppublish/value.h"

namespace CPPPUBLISH_NAMESPACE {
Value::Value() { _c_value = cpublish_value_new_none(); }

Value::Value(const Value &other) {
  _c_value = cpublish_value_clone(other.c_ptr(), nullptr);
}

Value::Value(bool value) { _c_value = cpublish_value_new_bool(value); }

Value::Value(int64_t value) { _c_value = cpublish_value_new_int(value); }

Value::Value(double value) { _c_value = cpublish_value_new_float(value); }

Value::Value(const std::string &value) {
  _c_value = cpublish_value_new_string(value.c_str());
}

Value::Value(const std::vector<Value> &value,
             CPPPUBLISH_NAMESPACE::Status &status) {
  _c_value = cpublish_value_new_array_with_capacity(value.size());

  for (auto &v : value) {
    cpublish_value_array_push(_c_value, v._c_value, (CPublishStatus *)&status);

    if (status.is_error()) {
      break;
    }
  }
}

Value::Value(const std::map<std::string, Value> &value,
             CPPPUBLISH_NAMESPACE::Status &status) {
  _c_value = cpublish_value_new_object_with_capacity(value.size());

  for (auto &v : value) {
    cpublish_value_object_insert(_c_value, v.first.c_str(), v.second._c_value,
                                 (CPublishStatus *)&status);

    if (status.is_error()) {
      break;
    }
  }
}

Value::~Value() {
  if (_c_value) {
    cpublish_value_destroy(_c_value);
  }
}

bool Value::operator==(const Value &other) const {
  return cpublish_value_eq(_c_value, other._c_value, nullptr);
}

bool Value::operator==(const ValueView &other) const {
  return cpublish_value_eq(_c_value, other.c_ptr(), nullptr);
}

bool Value::operator==(const bool other) const {
  CPPPUBLISH_NAMESPACE::Status status;

  if (type(status) != Value::Type::Boolean) {
    return false;
  }

  return value_bool(status) == other;
}

bool Value::operator==(const int64_t other) const {
  CPPPUBLISH_NAMESPACE::Status status;

  if (type(status) != Value::Type::Integer) {
    return false;
  }

  return value_int(status) == other;
}

bool Value::operator==(const double other) const {
  CPPPUBLISH_NAMESPACE::Status status;

  if (type(status) != Value::Type::Float) {
    return false;
  }

  return value_float(status) == other;
}

bool Value::operator==(const std::string other) const {
  CPPPUBLISH_NAMESPACE::Status status;

  if (type(status) != Value::Type::String) {
    return false;
  }

  return value_string(status) == other;
}

bool Value::operator==(
    const std::vector<CPPPUBLISH_NAMESPACE::Value> other) const {
  CPPPUBLISH_NAMESPACE::Status status;

  if (type(status) != Value::Type::Array) {
    return false;
  } else if (value_array_len(status) != other.size()) {
    return false;
  }

  for (size_t i = 0; i < other.size(); i++) {
    if (!(value_array_get(i, status) == other[i])) {
      return false;
    }
  }

  return true;
}

bool Value::operator==(
    const std::map<std::string, CPPPUBLISH_NAMESPACE::Value> other) const {
  CPPPUBLISH_NAMESPACE::Status status;

  if (type(status) != Value::Type::Object) {
    return false;
  } else if (value_array_len(status) != other.size()) {
    return false;
  }

  CPPPUBLISH_NAMESPACE::ValueIterObject iter = value_iter_object(status);

  for (; !iter.is_done(status); iter.next(status)) {
    std::string key = std::string{iter.key(status)};
    if (other.find(key) == other.end()) {
      return false;
    } else if (!(iter.value(status) == other.at(key))) {
      return false;
    }
  }

  return true;
}

const Value::Type Value::type(CPPPUBLISH_NAMESPACE::Status &status) const {
  return (Value::Type)cpublish_value_type(_c_value, (CPublishStatus *)&status);
}

bool Value::value_bool(CPPPUBLISH_NAMESPACE::Status &status) const {
  return cpublish_value_bool(_c_value, (CPublishStatus *)&status);
}

int64_t Value::value_int(CPPPUBLISH_NAMESPACE::Status &status) const {
  return cpublish_value_int(_c_value, (CPublishStatus *)&status);
}

double Value::value_float(CPPPUBLISH_NAMESPACE::Status &status) const {
  return cpublish_value_float(_c_value, (CPublishStatus *)&status);
}

std::string Value::value_string(CPPPUBLISH_NAMESPACE::Status &status) const {
  CPublishString value =
      cpublish_value_string(_c_value, (CPublishStatus *)&status);
  std::string out_value = value.string;
  cpublish_string_destroy(&value);

  return out_value;
}

size_t Value::value_array_len(CPPPUBLISH_NAMESPACE::Status &status) const {
  return cpublish_value_array_len(_c_value, (CPublishStatus *)&status);
}

const ValueView
Value::value_array_get(size_t index,
                       CPPPUBLISH_NAMESPACE::Status &status) const {
  return cpublish_value_array_get(_c_value, index, (CPublishStatus *)&status);
}

void Value::value_array_push(const Value &value,
                             CPPPUBLISH_NAMESPACE::Status &status) {
  cpublish_value_array_push(_c_value, value._c_value,
                            (CPublishStatus *)&status);
}

ValueIterArray
Value::value_iter_array(CPPPUBLISH_NAMESPACE::Status &status) const {
  return ValueIterArray(*this, status);
}

size_t Value::value_object_len(CPPPUBLISH_NAMESPACE::Status &status) const {
  return cpublish_value_object_len(_c_value, (CPublishStatus *)&status);
}

const ValueView
Value::value_object_get(const std::string &key,
                        CPPPUBLISH_NAMESPACE::Status &status) const {
  return cpublish_value_object_get(_c_value, key.c_str(),
                                   (CPublishStatus *)&status);
}

void Value::value_object_insert(const std::string &key, const Value &value,
                                CPPPUBLISH_NAMESPACE::Status &status) {
  cpublish_value_object_insert(_c_value, key.c_str(), value._c_value,
                               (CPublishStatus *)&status);
}

ValueIterObject
Value::value_iter_object(CPPPUBLISH_NAMESPACE::Status &status) const {
  return ValueIterObject(*this, status);
}

const CPublishValue *Value::c_ptr() const { return _c_value; }
} // namespace CPPPUBLISH_NAMESPACE
