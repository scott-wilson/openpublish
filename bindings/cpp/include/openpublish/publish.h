#pragma once

extern "C" {
#include <copenpublish.h>
}

#include <optional>

#include "openpublish/context.h"
#include "openpublish/context_view.h"
#include "openpublish/status.h"
#include "openpublish/value.h"

namespace CPPOPENPUBLISH_NAMESPACE {
class BasePublish : private OpenPublishBasePublish {
public:
  BasePublish();
  ~BasePublish();

  virtual std::optional<CPPOPENPUBLISH_NAMESPACE::Context>
  pre_publish(const CPPOPENPUBLISH_NAMESPACE::ContextView &context,
              CPPOPENPUBLISH_NAMESPACE::Status &status);

  virtual void
  rollback_pre_publish(const CPPOPENPUBLISH_NAMESPACE::ContextView &context,
                       CPPOPENPUBLISH_NAMESPACE::Status &status);

  virtual std::optional<CPPOPENPUBLISH_NAMESPACE::Context>
  publish(const CPPOPENPUBLISH_NAMESPACE::ContextView &context,
          CPPOPENPUBLISH_NAMESPACE::Status &status) = 0;

  virtual void
  rollback_publish(const CPPOPENPUBLISH_NAMESPACE::ContextView &context,
                   CPPOPENPUBLISH_NAMESPACE::Status &status);

  virtual std::optional<CPPOPENPUBLISH_NAMESPACE::Context>
  post_publish(const CPPOPENPUBLISH_NAMESPACE::ContextView &context,
               CPPOPENPUBLISH_NAMESPACE::Status &status);

  virtual void
  rollback_post_publish(const CPPOPENPUBLISH_NAMESPACE::ContextView &context,
                        CPPOPENPUBLISH_NAMESPACE::Status &status);

private:
  static OpenPublishContext *pre_publish_impl(const OpenPublishBasePublish *publish,
                                           const OpenPublishContext *context,
                                           OpenPublishStatus *status);

  static void rollback_pre_publish_impl(const OpenPublishBasePublish *publish,
                                        const OpenPublishContext *context,
                                        OpenPublishStatus *status);

  static OpenPublishContext *publish_impl(const OpenPublishBasePublish *publish,
                                       const OpenPublishContext *context,
                                       OpenPublishStatus *status);

  static void rollback_publish_impl(const OpenPublishBasePublish *publish,
                                    const OpenPublishContext *context,
                                    OpenPublishStatus *status);

  static OpenPublishContext *post_publish_impl(const OpenPublishBasePublish *publish,
                                            const OpenPublishContext *context,
                                            OpenPublishStatus *status);

  static void rollback_post_publish_impl(const OpenPublishBasePublish *publish,
                                         const OpenPublishContext *context,
                                         OpenPublishStatus *status);
};
} // namespace CPPOPENPUBLISH_NAMESPACE
