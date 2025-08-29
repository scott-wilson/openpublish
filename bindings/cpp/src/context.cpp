#include "cpppublish/context.h"

namespace CPPPUBLISH_NAMESPACE {
Context::Context() { _c_context = cpublish_context_new(); };

Context::Context(CPublishContext *c_context) : _c_context(c_context) {}

Context::Context(const Context &other) {
  _c_context = cpublish_context_clone(other._c_context, NULL);
}

Context::~Context() { cpublish_context_destroy(_c_context); }

const CPPPUBLISH_NAMESPACE::ValueView
Context::get(const std::string &key,
             CPPPUBLISH_NAMESPACE::Status &status) const {
  return cpublish_context_get(_c_context, key.c_str(),
                              (CPublishStatus *)&status);
}

void Context::set(const std::string &key,
                  const CPPPUBLISH_NAMESPACE::Value &value,
                  CPPPUBLISH_NAMESPACE::Status &status) {
  cpublish_context_set(_c_context, key.c_str(), value.c_ptr(),
                       (CPublishStatus *)&status);
}

void Context::set(const std::string &key,
                  CPPPUBLISH_NAMESPACE::Status &status) {
  cpublish_context_set_none(_c_context, key.c_str(), (CPublishStatus *)&status);
}

void Context::set(const std::string &key, bool value,
                  CPPPUBLISH_NAMESPACE::Status &status) {
  cpublish_context_set_bool(_c_context, key.c_str(), value,
                            (CPublishStatus *)&status);
}

void Context::set(const std::string &key, int64_t value,
                  CPPPUBLISH_NAMESPACE::Status &status) {
  cpublish_context_set_int(_c_context, key.c_str(), value,
                           (CPublishStatus *)&status);
}

void Context::set(const std::string &key, double value,
                  CPPPUBLISH_NAMESPACE::Status &status) {
  cpublish_context_set_float(_c_context, key.c_str(), value,
                             (CPublishStatus *)&status);
}

void Context::set(const std::string &key, const std::string &value,
                  CPPPUBLISH_NAMESPACE::Status &status) {
  cpublish_context_set_string(_c_context, key.c_str(), value.c_str(),
                              (CPublishStatus *)&status);
}

void Context::set(const std::string &key,
                  const std::vector<CPPPUBLISH_NAMESPACE::Value> &value,
                  CPPPUBLISH_NAMESPACE::Status &status) {
  CPPPUBLISH_NAMESPACE::Value out_value(value, status);

  if (status.is_error()) {
    return;
  }

  cpublish_context_set(_c_context, key.c_str(), out_value.c_ptr(),
                       (CPublishStatus *)&status);
}

void Context::set(
    const std::string &key,
    const std::map<std::string, CPPPUBLISH_NAMESPACE::Value> &value,
    CPPPUBLISH_NAMESPACE::Status &status) {
  CPPPUBLISH_NAMESPACE::Value out_value(value, status);

  if (status.is_error()) {
    return;
  }

  cpublish_context_set(_c_context, key.c_str(), out_value.c_ptr(),
                       (CPublishStatus *)&status);
}

size_t Context::len(CPPPUBLISH_NAMESPACE::Status &status) const {
  return cpublish_context_len(_c_context, (CPublishStatus *)&status);
}

bool Context::is_empty(CPPPUBLISH_NAMESPACE::Status &status) const {
  return cpublish_context_is_empty(_c_context, (CPublishStatus *)&status);
}

Context Context::clone(CPPPUBLISH_NAMESPACE::Status &status) const {
  return cpublish_context_clone(_c_context, (CPublishStatus *)&status);
}

const CPublishContext *Context::c_ptr() const { return _c_context; }

CPublishContext *Context::c_ptr_mut() const { return _c_context; }
} // namespace CPPPUBLISH_NAMESPACE
