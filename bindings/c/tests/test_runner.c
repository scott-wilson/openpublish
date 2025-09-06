#include <float.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <cmocka.h>

#include "copenpublish.h"
#include "test_utils.h"

OpenPublishContext *
pre_publish_should_pass(const OpenPublishBasePublish *publish,
                        const OpenPublishContext *context,
                        OpenPublishStatus *status) {
  openpublish_status_ok(status);
  assert_non_null(publish);
  assert_non_null(context);
  assert_non_null(status);

  OpenPublishContext *ctx = openpublish_context_clone(context, status);

  if (status != NULL && status->status == OpenPublishStatusTypeError) {
    return NULL;
  }

  openpublish_context_set_string(ctx, "pre_publish_key", "pre_publish_value",
                                 status);

  if (status != NULL && status->status == OpenPublishStatusTypeError) {
    return NULL;
  }

  return ctx;
}

void rollback_pre_publish_should_pass(const OpenPublishBasePublish *publish,
                                      const OpenPublishContext *context,
                                      OpenPublishStatus *status) {
  openpublish_status_ok(status);
  assert_non_null(publish);
  assert_non_null(context);
  assert_non_null(status);

  openpublish_status_ok(status);
}

OpenPublishContext *publish_should_pass(const OpenPublishBasePublish *publish,
                                        const OpenPublishContext *context,
                                        OpenPublishStatus *status) {
  openpublish_status_ok(status);
  assert_non_null(publish);
  assert_non_null(context);
  assert_non_null(status);

  OpenPublishContext *ctx = openpublish_context_clone(context, status);

  if (status != NULL && status->status == OpenPublishStatusTypeError) {
    return NULL;
  }

  openpublish_context_set_string(ctx, "publish_key", "publish_value", status);

  if (status != NULL && status->status == OpenPublishStatusTypeError) {
    return NULL;
  }

  return ctx;
}

void rollback_publish_should_pass(const OpenPublishBasePublish *publish,
                                  const OpenPublishContext *context,
                                  OpenPublishStatus *status) {
  openpublish_status_ok(status);
  assert_non_null(publish);
  assert_non_null(context);
  assert_non_null(status);

  openpublish_status_ok(status);
}

OpenPublishContext *
post_publish_should_pass(const OpenPublishBasePublish *publish,
                         const OpenPublishContext *context,
                         OpenPublishStatus *status) {
  openpublish_status_ok(status);
  assert_non_null(publish);
  assert_non_null(context);
  assert_non_null(status);

  OpenPublishContext *ctx = openpublish_context_clone(context, status);

  if (status != NULL && status->status == OpenPublishStatusTypeError) {
    return NULL;
  }

  openpublish_context_set_string(ctx, "post_publish_key", "post_publish_value",
                                 status);

  if (status != NULL && status->status == OpenPublishStatusTypeError) {
    return NULL;
  }

  return ctx;
}

void rollback_post_publish_should_pass(const OpenPublishBasePublish *publish,
                                       const OpenPublishContext *context,
                                       OpenPublishStatus *status) {
  openpublish_status_ok(status);
  assert_non_null(publish);
  assert_non_null(context);
  assert_non_null(status);

  openpublish_status_ok(status);
}

/*
----------------------------------------------------------------------------
  Checks
*/
static void test_run_success(void **state) {
  OpenPublishBasePublish publish = openpublish_publish_new_default();
  publish.pre_publish_fn = pre_publish_should_pass;
  publish.rollback_pre_publish_fn = rollback_pre_publish_should_pass;
  publish.publish_fn = publish_should_pass;
  publish.rollback_publish_fn = rollback_publish_should_pass;
  publish.post_publish_fn = post_publish_should_pass;
  publish.rollback_post_publish_fn = rollback_post_publish_should_pass;

  OpenPublishStatus status;

  OpenPublishContext *context = openpublish_run(&publish, &status);
  validate_status_ok(&status);
  assert_non_null(context);

  const OpenPublishValue *pre_publish_value =
      openpublish_context_get(context, "pre_publish_key", &status);
  validate_status_ok(&status);
  assert_non_null(pre_publish_value);
  OpenPublishString pre_publish_result =
      openpublish_value_string(pre_publish_value, &status);
  validate_status_ok(&status);
  assert_string_equal(pre_publish_result.string, "pre_publish_value");
  openpublish_string_destroy(&pre_publish_result);

  const OpenPublishValue *publish_value =
      openpublish_context_get(context, "publish_key", &status);
  validate_status_ok(&status);
  assert_non_null(publish_value);
  OpenPublishString publish_result =
      openpublish_value_string(publish_value, &status);
  validate_status_ok(&status);
  assert_string_equal(publish_result.string, "publish_value");
  openpublish_string_destroy(&publish_result);

  const OpenPublishValue *post_publish_value =
      openpublish_context_get(context, "post_publish_key", &status);
  validate_status_ok(&status);
  assert_non_null(post_publish_value);
  OpenPublishString post_publish_result =
      openpublish_value_string(post_publish_value, &status);
  validate_status_ok(&status);
  assert_string_equal(post_publish_result.string, "post_publish_value");
  openpublish_string_destroy(&post_publish_result);

  openpublish_context_destroy(context);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_run_success),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
