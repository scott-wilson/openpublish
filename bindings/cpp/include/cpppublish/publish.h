#pragma once

extern "C" {
#include <cpublish.h>
}

#include <optional>

#include "cpppublish/context.h"
#include "cpppublish/context_view.h"
#include "cpppublish/status.h"
#include "cpppublish/value.h"

namespace CPPPUBLISH_NAMESPACE {
class BasePublish : private CPublishBasePublish {
public:
  BasePublish();
  ~BasePublish();

  virtual std::optional<CPPPUBLISH_NAMESPACE::Context>
  pre_publish(const CPPPUBLISH_NAMESPACE::ContextView &context,
              CPPPUBLISH_NAMESPACE::Status &status);

  virtual void
  rollback_pre_publish(const CPPPUBLISH_NAMESPACE::ContextView &context,
                       CPPPUBLISH_NAMESPACE::Status &status);

  virtual std::optional<CPPPUBLISH_NAMESPACE::Context>
  publish(const CPPPUBLISH_NAMESPACE::ContextView &context,
          CPPPUBLISH_NAMESPACE::Status &status) = 0;

  virtual void
  rollback_publish(const CPPPUBLISH_NAMESPACE::ContextView &context,
                   CPPPUBLISH_NAMESPACE::Status &status);

  virtual std::optional<CPPPUBLISH_NAMESPACE::Context>
  post_publish(const CPPPUBLISH_NAMESPACE::ContextView &context,
               CPPPUBLISH_NAMESPACE::Status &status);

  virtual void
  rollback_post_publish(const CPPPUBLISH_NAMESPACE::ContextView &context,
                        CPPPUBLISH_NAMESPACE::Status &status);

private:
  static CPublishContext *pre_publish_impl(const CPublishBasePublish *publish,
                                           const CPublishContext *context,
                                           CPublishStatus *status);

  static void rollback_pre_publish_impl(const CPublishBasePublish *publish,
                                        const CPublishContext *context,
                                        CPublishStatus *status);

  static CPublishContext *publish_impl(const CPublishBasePublish *publish,
                                       const CPublishContext *context,
                                       CPublishStatus *status);

  static void rollback_publish_impl(const CPublishBasePublish *publish,
                                    const CPublishContext *context,
                                    CPublishStatus *status);

  static CPublishContext *post_publish_impl(const CPublishBasePublish *publish,
                                            const CPublishContext *context,
                                            CPublishStatus *status);

  static void rollback_post_publish_impl(const CPublishBasePublish *publish,
                                         const CPublishContext *context,
                                         CPublishStatus *status);
};
} // namespace CPPPUBLISH_NAMESPACE
