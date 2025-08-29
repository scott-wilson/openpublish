#include "cpppublish/value_view.h"

namespace CPPPUBLISH_NAMESPACE {
ValueView::ValueView(const CPublishValue *c_value) : _c_value(c_value) {}
ValueView::ValueView(const Value &value) : _c_value(value.c_ptr()) {}
ValueView::~ValueView() {}

bool ValueView::operator==(const ValueView &other) const {
  return cpublish_value_eq(_c_value, other.c_ptr(), nullptr);
}

bool ValueView::operator==(const Value &other) const {
  return cpublish_value_eq(_c_value, other.c_ptr(), nullptr);
}

bool ValueView::operator==(const bool other) const {
  CPPPUBLISH_NAMESPACE::Status status;

  if (type(status) != ValueView::Type::Boolean) {
    return false;
  }

  return value_bool(status) == other;
}

bool ValueView::operator==(const int64_t other) const {
  CPPPUBLISH_NAMESPACE::Status status;

  if (type(status) != ValueView::Type::Integer) {
    return false;
  }

  return value_int(status) == other;
}

bool ValueView::operator==(const double other) const {
  CPPPUBLISH_NAMESPACE::Status status;

  if (type(status) != ValueView::Type::Float) {
    return false;
  }

  return value_float(status) == other;
}

bool ValueView::operator==(const std::string other) const {
  CPPPUBLISH_NAMESPACE::Status status;

  if (type(status) != ValueView::Type::String) {
    return false;
  }

  return value_string(status) == other;
}

bool ValueView::operator==(
    const std::vector<CPPPUBLISH_NAMESPACE::Value> other) const {
  CPPPUBLISH_NAMESPACE::Status status;

  if (type(status) != ValueView::Type::Array) {
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

bool ValueView::operator==(
    const std::map<std::string, CPPPUBLISH_NAMESPACE::Value> other) const {
  CPPPUBLISH_NAMESPACE::Status status;

  if (type(status) != ValueView::Type::Object) {
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

const ValueView::Type
ValueView::type(CPPPUBLISH_NAMESPACE::Status &status) const {
  return (ValueView::Type)cpublish_value_type(_c_value,
                                              (CPublishStatus *)&status);
}

bool ValueView::value_bool(CPPPUBLISH_NAMESPACE::Status &status) const {
  return cpublish_value_bool(_c_value, (CPublishStatus *)&status);
}

int64_t ValueView::value_int(CPPPUBLISH_NAMESPACE::Status &status) const {
  return cpublish_value_int(_c_value, (CPublishStatus *)&status);
}

double ValueView::value_float(CPPPUBLISH_NAMESPACE::Status &status) const {
  return cpublish_value_float(_c_value, (CPublishStatus *)&status);
}

std::string
ValueView::value_string(CPPPUBLISH_NAMESPACE::Status &status) const {
  CPublishString value =
      cpublish_value_string(_c_value, (CPublishStatus *)&status);
  std::string out_value = value.string;
  cpublish_string_destroy(&value);

  return out_value;
}

size_t ValueView::value_array_len(CPPPUBLISH_NAMESPACE::Status &status) const {
  return cpublish_value_array_len(_c_value, (CPublishStatus *)&status);
}

const ValueView
ValueView::value_array_get(size_t index,
                           CPPPUBLISH_NAMESPACE::Status &status) const {
  return cpublish_value_array_get(_c_value, index, (CPublishStatus *)&status);
}

ValueIterArray
ValueView::value_iter_array(CPPPUBLISH_NAMESPACE::Status &status) const {
  return ValueIterArray(*this, status);
}

size_t ValueView::value_object_len(CPPPUBLISH_NAMESPACE::Status &status) const {
  return cpublish_value_object_len(_c_value, (CPublishStatus *)&status);
}

const ValueView
ValueView::value_object_get(std::string &key,
                            CPPPUBLISH_NAMESPACE::Status &status) const {
  return cpublish_value_object_get(_c_value, key.c_str(),
                                   (CPublishStatus *)&status);
}

ValueIterObject
ValueView::value_iter_object(CPPPUBLISH_NAMESPACE::Status &status) const {
  return ValueIterObject(*this, status);
}

const CPublishValue *ValueView::c_ptr() const { return _c_value; }
} // namespace CPPPUBLISH_NAMESPACE
