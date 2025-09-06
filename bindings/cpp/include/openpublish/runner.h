#pragma once

extern "C" {
#include <copenpublish.h>
}

#include "openpublish/context.h"
#include "openpublish/publish.h"
#include "openpublish/status.h"

namespace CPPOPENPUBLISH_NAMESPACE {
const CPPOPENPUBLISH_NAMESPACE::Context
run(const CPPOPENPUBLISH_NAMESPACE::BasePublish &publish,
    CPPOPENPUBLISH_NAMESPACE::Status &status);
} // namespace CPPOPENPUBLISH_NAMESPACE
