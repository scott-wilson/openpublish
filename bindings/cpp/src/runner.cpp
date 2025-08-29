#include "cpppublish/runner.h"

namespace CPPPUBLISH_NAMESPACE {
const CPPPUBLISH_NAMESPACE::Context
run(const CPPPUBLISH_NAMESPACE::BasePublish &publish,
    CPPPUBLISH_NAMESPACE::Status &status) {
  return cpublish_run((const CPublishBasePublish *)&publish,
                      (CPublishStatus *)&status);
}
} // namespace CPPPUBLISH_NAMESPACE
