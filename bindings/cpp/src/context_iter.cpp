#include "openpublish/context_iter.h"

namespace CPPOPENPUBLISH_NAMESPACE {
ContextIter::ContextIter(const CPPOPENPUBLISH_NAMESPACE::Context &context,
                         CPPOPENPUBLISH_NAMESPACE::Status &status) {
  _iter = openpublish_context_iter((const OpenPublishContext *)&context,
                                (OpenPublishStatus *)&status);
}
ContextIter::~ContextIter() { openpublish_context_iter_destroy(_iter); }

bool ContextIter::is_done(CPPOPENPUBLISH_NAMESPACE::Status &status) const {
  return openpublish_context_iter_is_done(_iter, (OpenPublishStatus *)&status);
}

void ContextIter::next(CPPOPENPUBLISH_NAMESPACE::Status &status) {
  openpublish_context_iter_next(_iter, (OpenPublishStatus *)&status);
}

const ValueView ContextIter::value(CPPOPENPUBLISH_NAMESPACE::Status &status) const {
  return openpublish_context_iter_value(_iter, (OpenPublishStatus *)&status);
}
} // namespace CPPOPENPUBLISH_NAMESPACE
