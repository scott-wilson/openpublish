#include "cpppublish/publish.h"

namespace CPPPUBLISH_NAMESPACE {
BasePublish::BasePublish() {
  this->pre_publish_fn = pre_publish_impl;
  this->rollback_pre_publish_fn = rollback_pre_publish_impl;
  this->publish_fn = publish_impl;
  this->rollback_publish_fn = rollback_publish_impl;
  this->post_publish_fn = post_publish_impl;
  this->rollback_post_publish_fn = rollback_post_publish_impl;
}

BasePublish::~BasePublish() {}

std::optional<CPPPUBLISH_NAMESPACE::Context>
BasePublish::pre_publish(const CPPPUBLISH_NAMESPACE::ContextView &context,
                         CPPPUBLISH_NAMESPACE::Status &status) {
  status.set_ok();
  return std::nullopt;
}

void BasePublish::rollback_pre_publish(
    const CPPPUBLISH_NAMESPACE::ContextView &context,
    CPPPUBLISH_NAMESPACE::Status &status) {
  status.set_ok();
}

void BasePublish::rollback_publish(
    const CPPPUBLISH_NAMESPACE::ContextView &context,
    CPPPUBLISH_NAMESPACE::Status &status) {
  status.set_ok();
}

std::optional<CPPPUBLISH_NAMESPACE::Context>
BasePublish::post_publish(const CPPPUBLISH_NAMESPACE::ContextView &context,
                          CPPPUBLISH_NAMESPACE::Status &status) {
  status.set_ok();
  return std::nullopt;
}

void BasePublish::rollback_post_publish(
    const CPPPUBLISH_NAMESPACE::ContextView &context,
    CPPPUBLISH_NAMESPACE::Status &status) {
  status.set_ok();
}

CPublishContext *
BasePublish::pre_publish_impl(const CPublishBasePublish *publish,
                              const CPublishContext *context,
                              CPublishStatus *status) {
  CPPPUBLISH_NAMESPACE::Status *cpp_status =
      (CPPPUBLISH_NAMESPACE::Status *)status;
  std::optional<CPPPUBLISH_NAMESPACE::Context> cpp_out_context =
      ((CPPPUBLISH_NAMESPACE::BasePublish *)publish)
          ->pre_publish(context, *cpp_status);

  if (cpp_out_context) {
    return cpp_out_context.value().c_ptr_mut();
  } else {
    return nullptr;
  }
}

void BasePublish::rollback_pre_publish_impl(const CPublishBasePublish *publish,
                                            const CPublishContext *context,
                                            CPublishStatus *status) {
  CPPPUBLISH_NAMESPACE::Status *cpp_status =
      (CPPPUBLISH_NAMESPACE::Status *)status;
  ((CPPPUBLISH_NAMESPACE::BasePublish *)publish)
      ->rollback_pre_publish(context, *cpp_status);
}

CPublishContext *BasePublish::publish_impl(const CPublishBasePublish *publish,
                                           const CPublishContext *context,
                                           CPublishStatus *status) {
  CPPPUBLISH_NAMESPACE::Status *cpp_status =
      (CPPPUBLISH_NAMESPACE::Status *)status;
  std::optional<CPPPUBLISH_NAMESPACE::Context> cpp_out_context =
      ((CPPPUBLISH_NAMESPACE::BasePublish *)publish)
          ->publish(context, *cpp_status);

  if (cpp_out_context) {
    return cpp_out_context.value().c_ptr_mut();
  } else {
    return nullptr;
  }
}

void BasePublish::rollback_publish_impl(const CPublishBasePublish *publish,
                                        const CPublishContext *context,
                                        CPublishStatus *status) {
  CPPPUBLISH_NAMESPACE::Status *cpp_status =
      (CPPPUBLISH_NAMESPACE::Status *)status;
  ((CPPPUBLISH_NAMESPACE::BasePublish *)publish)
      ->rollback_publish(context, *cpp_status);
}

CPublishContext *
BasePublish::post_publish_impl(const CPublishBasePublish *publish,
                               const CPublishContext *context,
                               CPublishStatus *status) {
  CPPPUBLISH_NAMESPACE::Status *cpp_status =
      (CPPPUBLISH_NAMESPACE::Status *)status;
  std::optional<CPPPUBLISH_NAMESPACE::Context> cpp_out_context =
      ((CPPPUBLISH_NAMESPACE::BasePublish *)publish)
          ->post_publish(context, *cpp_status);

  if (cpp_out_context) {
    return cpp_out_context.value().c_ptr_mut();
  } else {
    return nullptr;
  }
}

void BasePublish::rollback_post_publish_impl(const CPublishBasePublish *publish,
                                             const CPublishContext *context,
                                             CPublishStatus *status) {
  CPPPUBLISH_NAMESPACE::Status *cpp_status =
      (CPPPUBLISH_NAMESPACE::Status *)status;
  ((CPPPUBLISH_NAMESPACE::BasePublish *)publish)
      ->rollback_post_publish(context, *cpp_status);
}
} // namespace CPPPUBLISH_NAMESPACE
