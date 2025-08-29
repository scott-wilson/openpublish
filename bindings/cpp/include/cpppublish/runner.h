#pragma once

extern "C" {
#include <cpublish.h>
}

#include "cpppublish/context.h"
#include "cpppublish/publish.h"
#include "cpppublish/status.h"

namespace CPPPUBLISH_NAMESPACE {
const CPPPUBLISH_NAMESPACE::Context
run(const CPPPUBLISH_NAMESPACE::BasePublish &publish,
    CPPPUBLISH_NAMESPACE::Status &status);
} // namespace CPPPUBLISH_NAMESPACE
