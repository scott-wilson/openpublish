#include "cpppublish/value_iter.h"
#include "cpppublish/value.h"

namespace CPPPUBLISH_NAMESPACE {
ValueIterArray::ValueIterArray(const Value &value,
                               CPPPUBLISH_NAMESPACE::Status &status) {
  _iter = cpublish_value_array_iter(value.c_ptr(), (CPublishStatus *)&status);
}

ValueIterArray::ValueIterArray(const ValueView &value,
                               CPPPUBLISH_NAMESPACE::Status &status) {
  _iter = cpublish_value_array_iter(value.c_ptr(), (CPublishStatus *)&status);
}

ValueIterArray::~ValueIterArray() { cpublish_value_iter_array_destroy(_iter); }

bool ValueIterArray::is_done(CPPPUBLISH_NAMESPACE::Status &status) const {
  return cpublish_value_iter_array_is_done(_iter, (CPublishStatus *)&status);
}

void ValueIterArray::next(CPPPUBLISH_NAMESPACE::Status &status) {
  cpublish_value_iter_array_next(_iter, (CPublishStatus *)&status);
}

const ValueView
ValueIterArray::value(CPPPUBLISH_NAMESPACE::Status &status) const {
  return cpublish_value_iter_array_value(_iter, (CPublishStatus *)&status);
}

ValueIterObject::ValueIterObject(const CPPPUBLISH_NAMESPACE::Value &value,
                                 CPPPUBLISH_NAMESPACE::Status &status) {
  _iter = cpublish_value_object_iter(value.c_ptr(), (CPublishStatus *)&status);
}

ValueIterObject::ValueIterObject(const CPPPUBLISH_NAMESPACE::ValueView &value,
                                 CPPPUBLISH_NAMESPACE::Status &status) {
  _iter = cpublish_value_object_iter(value.c_ptr(), (CPublishStatus *)&status);
}

ValueIterObject::~ValueIterObject() {
  cpublish_value_iter_object_destroy(_iter);
}

bool ValueIterObject::is_done(CPPPUBLISH_NAMESPACE::Status &status) const {
  return cpublish_value_iter_object_is_done(_iter, (CPublishStatus *)&status);
}

void ValueIterObject::next(CPPPUBLISH_NAMESPACE::Status &status) {
  cpublish_value_iter_object_next(_iter, (CPublishStatus *)&status);
}

const std::string_view
ValueIterObject::key(CPPPUBLISH_NAMESPACE::Status &status) const {
  CPublishStringView c_str =
      cpublish_value_iter_object_key(_iter, (CPublishStatus *)&status);

  return std::string_view(c_str.string);
}

const CPPPUBLISH_NAMESPACE::ValueView
ValueIterObject::value(CPPPUBLISH_NAMESPACE::Status &status) const {
  return cpublish_value_iter_object_value(_iter, (CPublishStatus *)&status);
}

} // namespace CPPPUBLISH_NAMESPACE
