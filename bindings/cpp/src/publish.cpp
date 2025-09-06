#include "openpublish/publish.h"

namespace CPPOPENPUBLISH_NAMESPACE {
BasePublish::BasePublish() {
  this->pre_publish_fn = pre_publish_impl;
  this->rollback_pre_publish_fn = rollback_pre_publish_impl;
  this->publish_fn = publish_impl;
  this->rollback_publish_fn = rollback_publish_impl;
  this->post_publish_fn = post_publish_impl;
  this->rollback_post_publish_fn = rollback_post_publish_impl;
}

BasePublish::~BasePublish() {}

std::optional<CPPOPENPUBLISH_NAMESPACE::Context>
BasePublish::pre_publish(const CPPOPENPUBLISH_NAMESPACE::ContextView &context,
                         CPPOPENPUBLISH_NAMESPACE::Status &status) {
  status.set_ok();
  return std::nullopt;
}

void BasePublish::rollback_pre_publish(
    const CPPOPENPUBLISH_NAMESPACE::ContextView &context,
    CPPOPENPUBLISH_NAMESPACE::Status &status) {
  status.set_ok();
}

void BasePublish::rollback_publish(
    const CPPOPENPUBLISH_NAMESPACE::ContextView &context,
    CPPOPENPUBLISH_NAMESPACE::Status &status) {
  status.set_ok();
}

std::optional<CPPOPENPUBLISH_NAMESPACE::Context>
BasePublish::post_publish(const CPPOPENPUBLISH_NAMESPACE::ContextView &context,
                          CPPOPENPUBLISH_NAMESPACE::Status &status) {
  status.set_ok();
  return std::nullopt;
}

void BasePublish::rollback_post_publish(
    const CPPOPENPUBLISH_NAMESPACE::ContextView &context,
    CPPOPENPUBLISH_NAMESPACE::Status &status) {
  status.set_ok();
}

OpenPublishContext *
BasePublish::pre_publish_impl(const OpenPublishBasePublish *publish,
                              const OpenPublishContext *context,
                              OpenPublishStatus *status) {
  CPPOPENPUBLISH_NAMESPACE::Status *cpp_status =
      (CPPOPENPUBLISH_NAMESPACE::Status *)status;
  std::optional<CPPOPENPUBLISH_NAMESPACE::Context> cpp_out_context =
      ((CPPOPENPUBLISH_NAMESPACE::BasePublish *)publish)
          ->pre_publish(context, *cpp_status);

  if (cpp_out_context) {
    return cpp_out_context.value().c_ptr_mut();
  } else {
    return nullptr;
  }
}

void BasePublish::rollback_pre_publish_impl(const OpenPublishBasePublish *publish,
                                            const OpenPublishContext *context,
                                            OpenPublishStatus *status) {
  CPPOPENPUBLISH_NAMESPACE::Status *cpp_status =
      (CPPOPENPUBLISH_NAMESPACE::Status *)status;
  ((CPPOPENPUBLISH_NAMESPACE::BasePublish *)publish)
      ->rollback_pre_publish(context, *cpp_status);
}

OpenPublishContext *BasePublish::publish_impl(const OpenPublishBasePublish *publish,
                                           const OpenPublishContext *context,
                                           OpenPublishStatus *status) {
  CPPOPENPUBLISH_NAMESPACE::Status *cpp_status =
      (CPPOPENPUBLISH_NAMESPACE::Status *)status;
  std::optional<CPPOPENPUBLISH_NAMESPACE::Context> cpp_out_context =
      ((CPPOPENPUBLISH_NAMESPACE::BasePublish *)publish)
          ->publish(context, *cpp_status);

  if (cpp_out_context) {
    return cpp_out_context.value().c_ptr_mut();
  } else {
    return nullptr;
  }
}

void BasePublish::rollback_publish_impl(const OpenPublishBasePublish *publish,
                                        const OpenPublishContext *context,
                                        OpenPublishStatus *status) {
  CPPOPENPUBLISH_NAMESPACE::Status *cpp_status =
      (CPPOPENPUBLISH_NAMESPACE::Status *)status;
  ((CPPOPENPUBLISH_NAMESPACE::BasePublish *)publish)
      ->rollback_publish(context, *cpp_status);
}

OpenPublishContext *
BasePublish::post_publish_impl(const OpenPublishBasePublish *publish,
                               const OpenPublishContext *context,
                               OpenPublishStatus *status) {
  CPPOPENPUBLISH_NAMESPACE::Status *cpp_status =
      (CPPOPENPUBLISH_NAMESPACE::Status *)status;
  std::optional<CPPOPENPUBLISH_NAMESPACE::Context> cpp_out_context =
      ((CPPOPENPUBLISH_NAMESPACE::BasePublish *)publish)
          ->post_publish(context, *cpp_status);

  if (cpp_out_context) {
    return cpp_out_context.value().c_ptr_mut();
  } else {
    return nullptr;
  }
}

void BasePublish::rollback_post_publish_impl(const OpenPublishBasePublish *publish,
                                             const OpenPublishContext *context,
                                             OpenPublishStatus *status) {
  CPPOPENPUBLISH_NAMESPACE::Status *cpp_status =
      (CPPOPENPUBLISH_NAMESPACE::Status *)status;
  ((CPPOPENPUBLISH_NAMESPACE::BasePublish *)publish)
      ->rollback_post_publish(context, *cpp_status);
}
} // namespace CPPOPENPUBLISH_NAMESPACE
