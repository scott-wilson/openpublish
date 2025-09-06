#include "openpublish/runner.h"

namespace CPPOPENPUBLISH_NAMESPACE {
const CPPOPENPUBLISH_NAMESPACE::Context
run(const CPPOPENPUBLISH_NAMESPACE::BasePublish &publish,
    CPPOPENPUBLISH_NAMESPACE::Status &status) {
  return openpublish_run((const OpenPublishBasePublish *)&publish,
                      (OpenPublishStatus *)&status);
}
} // namespace CPPOPENPUBLISH_NAMESPACE
