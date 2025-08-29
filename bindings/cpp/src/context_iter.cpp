#include "cpppublish/context_iter.h"

namespace CPPPUBLISH_NAMESPACE {
ContextIter::ContextIter(const CPPPUBLISH_NAMESPACE::Context &context,
                         CPPPUBLISH_NAMESPACE::Status &status) {
  _iter = cpublish_context_iter((const CPublishContext *)&context,
                                (CPublishStatus *)&status);
}
ContextIter::~ContextIter() { cpublish_context_iter_destroy(_iter); }

bool ContextIter::is_done(CPPPUBLISH_NAMESPACE::Status &status) const {
  return cpublish_context_iter_is_done(_iter, (CPublishStatus *)&status);
}

void ContextIter::next(CPPPUBLISH_NAMESPACE::Status &status) {
  cpublish_context_iter_next(_iter, (CPublishStatus *)&status);
}

const ValueView ContextIter::value(CPPPUBLISH_NAMESPACE::Status &status) const {
  return cpublish_context_iter_value(_iter, (CPublishStatus *)&status);
}
} // namespace CPPPUBLISH_NAMESPACE
