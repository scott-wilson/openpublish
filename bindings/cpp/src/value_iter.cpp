#include "openpublish/value_iter.h"
#include "openpublish/value.h"

namespace CPPOPENPUBLISH_NAMESPACE {
ValueIterArray::ValueIterArray(const Value &value,
                               CPPOPENPUBLISH_NAMESPACE::Status &status) {
  _iter = openpublish_value_array_iter(value.c_ptr(), (OpenPublishStatus *)&status);
}

ValueIterArray::ValueIterArray(const ValueView &value,
                               CPPOPENPUBLISH_NAMESPACE::Status &status) {
  _iter = openpublish_value_array_iter(value.c_ptr(), (OpenPublishStatus *)&status);
}

ValueIterArray::~ValueIterArray() { openpublish_value_iter_array_destroy(_iter); }

bool ValueIterArray::is_done(CPPOPENPUBLISH_NAMESPACE::Status &status) const {
  return openpublish_value_iter_array_is_done(_iter, (OpenPublishStatus *)&status);
}

void ValueIterArray::next(CPPOPENPUBLISH_NAMESPACE::Status &status) {
  openpublish_value_iter_array_next(_iter, (OpenPublishStatus *)&status);
}

const ValueView
ValueIterArray::value(CPPOPENPUBLISH_NAMESPACE::Status &status) const {
  return openpublish_value_iter_array_value(_iter, (OpenPublishStatus *)&status);
}

ValueIterObject::ValueIterObject(const CPPOPENPUBLISH_NAMESPACE::Value &value,
                                 CPPOPENPUBLISH_NAMESPACE::Status &status) {
  _iter = openpublish_value_object_iter(value.c_ptr(), (OpenPublishStatus *)&status);
}

ValueIterObject::ValueIterObject(const CPPOPENPUBLISH_NAMESPACE::ValueView &value,
                                 CPPOPENPUBLISH_NAMESPACE::Status &status) {
  _iter = openpublish_value_object_iter(value.c_ptr(), (OpenPublishStatus *)&status);
}

ValueIterObject::~ValueIterObject() {
  openpublish_value_iter_object_destroy(_iter);
}

bool ValueIterObject::is_done(CPPOPENPUBLISH_NAMESPACE::Status &status) const {
  return openpublish_value_iter_object_is_done(_iter, (OpenPublishStatus *)&status);
}

void ValueIterObject::next(CPPOPENPUBLISH_NAMESPACE::Status &status) {
  openpublish_value_iter_object_next(_iter, (OpenPublishStatus *)&status);
}

const std::string_view
ValueIterObject::key(CPPOPENPUBLISH_NAMESPACE::Status &status) const {
  OpenPublishStringView c_str =
      openpublish_value_iter_object_key(_iter, (OpenPublishStatus *)&status);

  return std::string_view(c_str.string);
}

const CPPOPENPUBLISH_NAMESPACE::ValueView
ValueIterObject::value(CPPOPENPUBLISH_NAMESPACE::Status &status) const {
  return openpublish_value_iter_object_value(_iter, (OpenPublishStatus *)&status);
}

} // namespace CPPOPENPUBLISH_NAMESPACE
