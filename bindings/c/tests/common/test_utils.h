#include <cmocka.h>

#include "copenpublish.h"

void _validate_status_ok(const OpenPublishStatus *status,
                         const char *const file, const int line) {
  assert_non_null(status);

  if (status->status != OpenPublishStatusTypeOk) {
    fprintf(stderr, "file: %s:%d\n", file, line);
    fprintf(stderr, "status: %d\n", status->status);
    fprintf(stderr, "message: %s\n", status->message);
  }

  assert_int_equal(status->status, OpenPublishStatusTypeOk);
}

#define validate_status_ok(status)                                             \
  _validate_status_ok(status, __FILE__, __LINE__)
