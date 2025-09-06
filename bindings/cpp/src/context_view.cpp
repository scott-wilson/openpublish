#include "openpublish/context_view.h"

namespace CPPOPENPUBLISH_NAMESPACE {
ContextView::ContextView(const OpenPublishContext *context)
    : _c_context(context) {}
ContextView::ContextView(const CPPOPENPUBLISH_NAMESPACE::Context &context)
    : _c_context(context.c_ptr()) {}
ContextView::~ContextView() {}

const CPPOPENPUBLISH_NAMESPACE::ValueView
ContextView::get(std::string &key, CPPOPENPUBLISH_NAMESPACE::Status &status) {
  return openpublish_context_get(_c_context, key.c_str(),
                              (OpenPublishStatus *)&status);
}

size_t ContextView::len(CPPOPENPUBLISH_NAMESPACE::Status &status) const {
  return openpublish_context_len(_c_context, (OpenPublishStatus *)&status);
}

bool ContextView::is_empty(CPPOPENPUBLISH_NAMESPACE::Status &status) const {
  return openpublish_context_is_empty(_c_context, (OpenPublishStatus *)&status);
}

Context ContextView::clone(CPPOPENPUBLISH_NAMESPACE::Status &status) const {
  return openpublish_context_clone(_c_context, (OpenPublishStatus *)&status);
}

const OpenPublishContext *ContextView::c_ptr() const { return _c_context; }
} // namespace CPPOPENPUBLISH_NAMESPACE
