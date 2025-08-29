#include "cpppublish/context_view.h"

namespace CPPPUBLISH_NAMESPACE {
ContextView::ContextView(const CPublishContext *context)
    : _c_context(context) {}
ContextView::ContextView(const CPPPUBLISH_NAMESPACE::Context &context)
    : _c_context(context.c_ptr()) {}
ContextView::~ContextView() {}

const CPPPUBLISH_NAMESPACE::ValueView
ContextView::get(std::string &key, CPPPUBLISH_NAMESPACE::Status &status) {
  return cpublish_context_get(_c_context, key.c_str(),
                              (CPublishStatus *)&status);
}

size_t ContextView::len(CPPPUBLISH_NAMESPACE::Status &status) const {
  return cpublish_context_len(_c_context, (CPublishStatus *)&status);
}

bool ContextView::is_empty(CPPPUBLISH_NAMESPACE::Status &status) const {
  return cpublish_context_is_empty(_c_context, (CPublishStatus *)&status);
}

Context ContextView::clone(CPPPUBLISH_NAMESPACE::Status &status) const {
  return cpublish_context_clone(_c_context, (CPublishStatus *)&status);
}

const CPublishContext *ContextView::c_ptr() const { return _c_context; }
} // namespace CPPPUBLISH_NAMESPACE
