#include "openpublish/context.h"
#include "copenpublish.h"
#include "openpublish/status.h"

namespace CPPOPENPUBLISH_NAMESPACE {
Context::Context() { _c_context = openpublish_context_new(); };

Context::Context(OpenPublishContext *c_context) : _c_context(c_context) {}

Context::Context(const Context &other) {
  _c_context = openpublish_context_clone(other._c_context, nullptr);
}

Context::~Context() { openpublish_context_destroy(_c_context); }

const CPPOPENPUBLISH_NAMESPACE::ValueView
Context::get(const std::string &key,
             CPPOPENPUBLISH_NAMESPACE::Status &status) const {
  return openpublish_context_get(_c_context, key.c_str(),
                                 (OpenPublishStatus *)&status);
}

void Context::set(const std::string &key,
                  const CPPOPENPUBLISH_NAMESPACE::Value &value,
                  CPPOPENPUBLISH_NAMESPACE::Status &status) {
  openpublish_context_set(_c_context, key.c_str(), value.c_ptr(),
                          (OpenPublishStatus *)&status);
}

void Context::set(const std::string &key,
                  CPPOPENPUBLISH_NAMESPACE::Status &status) {
  openpublish_context_set_none(_c_context, key.c_str(),
                               (OpenPublishStatus *)&status);
}

void Context::set(const std::string &key, bool value,
                  CPPOPENPUBLISH_NAMESPACE::Status &status) {
  openpublish_context_set_bool(_c_context, key.c_str(), value,
                               (OpenPublishStatus *)&status);
}

void Context::set(const std::string &key, int64_t value,
                  CPPOPENPUBLISH_NAMESPACE::Status &status) {
  openpublish_context_set_int(_c_context, key.c_str(), value,
                              (OpenPublishStatus *)&status);
}

void Context::set(const std::string &key, double value,
                  CPPOPENPUBLISH_NAMESPACE::Status &status) {
  openpublish_context_set_float(_c_context, key.c_str(), value,
                                (OpenPublishStatus *)&status);
}

void Context::set(const std::string &key, const std::string &value,
                  CPPOPENPUBLISH_NAMESPACE::Status &status) {
  openpublish_context_set_string(_c_context, key.c_str(), value.c_str(),
                                 (OpenPublishStatus *)&status);
}

void Context::set(const std::string &key,
                  const std::vector<CPPOPENPUBLISH_NAMESPACE::Value> &value,
                  CPPOPENPUBLISH_NAMESPACE::Status &status) {
  CPPOPENPUBLISH_NAMESPACE::Value out_value(value, status);

  if (status.is_error()) {
    return;
  }

  openpublish_context_set(_c_context, key.c_str(), out_value.c_ptr(),
                          (OpenPublishStatus *)&status);
}

void Context::set(
    const std::string &key,
    const std::map<std::string, CPPOPENPUBLISH_NAMESPACE::Value> &value,
    CPPOPENPUBLISH_NAMESPACE::Status &status) {
  CPPOPENPUBLISH_NAMESPACE::Value out_value(value, status);

  if (status.is_error()) {
    return;
  }

  openpublish_context_set(_c_context, key.c_str(), out_value.c_ptr(),
                          (OpenPublishStatus *)&status);
}

size_t Context::len(CPPOPENPUBLISH_NAMESPACE::Status &status) const {
  return openpublish_context_len(_c_context, (OpenPublishStatus *)&status);
}

bool Context::is_empty(CPPOPENPUBLISH_NAMESPACE::Status &status) const {
  return openpublish_context_is_empty(_c_context, (OpenPublishStatus *)&status);
}

Context Context::clone(CPPOPENPUBLISH_NAMESPACE::Status &status) const {
  return openpublish_context_clone(_c_context, (OpenPublishStatus *)&status);
}

const OpenPublishContext *Context::c_ptr() const { return _c_context; }

OpenPublishContext *Context::c_ptr_mut() const { return _c_context; }
} // namespace CPPOPENPUBLISH_NAMESPACE
