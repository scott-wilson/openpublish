#include <iostream>

#include <gtest/gtest.h>

#include "cpppublish/status.h"

void _validate_status_ok(const CPPPUBLISH_NAMESPACE::Status &status,
                         const char *const file, const int line) {
  if (status.is_error()) {
    std::cerr << "file: " << file << ":" << line << std::endl;
    std::cerr << "message: " << status.message() << std::endl;
  }

  ASSERT_EQ(status.is_ok(), true);
}

#define validate_status_ok(status)                                             \
  _validate_status_ok(status, __FILE__, __LINE__)
