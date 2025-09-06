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

  openpublish_context_set_bool(ctx, "test", true, status);

  if (status != NULL && status->status == OpenPublishStatusTypeError) {
    return NULL;
  }

  return ctx;
}

OpenPublishContext *publish_should_fail(const OpenPublishBasePublish *publish,
                                        const OpenPublishContext *context,
                                        OpenPublishStatus *status) {
  openpublish_status_ok(status);
  assert_non_null(publish);
  assert_non_null(context);
  assert_non_null(status);

  openpublish_status_error(status, "Should fail");

  return NULL;
}

void rollback_should_pass(const OpenPublishBasePublish *publish,
                          const OpenPublishContext *context,
                          OpenPublishStatus *status) {
  openpublish_status_ok(status);
  assert_non_null(publish);
  assert_non_null(context);
  assert_non_null(status);

  openpublish_status_ok(status);
}

void rollback_should_fail(const OpenPublishBasePublish *publish,
                          const OpenPublishContext *context,
                          OpenPublishStatus *status) {
  openpublish_status_ok(status);
  assert_non_null(publish);
  assert_non_null(context);
  assert_non_null(status);

  openpublish_status_error(status, "Should fail");
}

/*
----------------------------------------------------------------------------
  Checks
*/
static void test_openpublish_publish_new_default_success(void **state) {
  OpenPublishBasePublish publish = openpublish_publish_new_default();

  assert_ptr_equal(publish.pre_publish_fn, openpublish_publish_default_publish);
  assert_ptr_equal(publish.rollback_pre_publish_fn,
                   openpublish_publish_default_rollback_publish);
  assert_ptr_equal(publish.publish_fn,
                   openpublish_publish_default_error_publish);
  assert_ptr_equal(publish.rollback_publish_fn,
                   openpublish_publish_default_rollback_publish);
  assert_ptr_equal(publish.post_publish_fn,
                   openpublish_publish_default_publish);
  assert_ptr_equal(publish.rollback_post_publish_fn,
                   openpublish_publish_default_rollback_publish);
}

static void test_openpublish_publish_pre_publish_success(void **state) {
  OpenPublishBasePublish publish = openpublish_publish_new_default();
  publish.pre_publish_fn = publish_should_pass;
  publish.rollback_pre_publish_fn = rollback_should_fail;
  publish.publish_fn = publish_should_fail;
  publish.rollback_publish_fn = rollback_should_fail;
  publish.post_publish_fn = publish_should_fail;
  publish.rollback_post_publish_fn = rollback_should_fail;
  OpenPublishContext *context = openpublish_context_new();
  OpenPublishStatus status;
  openpublish_status_ok(&status);

  OpenPublishContext *ctx =
      openpublish_publish_pre_publish(&publish, context, &status);
  validate_status_ok(&status);
  assert_non_null(ctx);
  const OpenPublishValue *result =
      openpublish_context_get(ctx, "test", &status);
  validate_status_ok(&status);
  assert_non_null(result);

  OpenPublishValueType type = openpublish_value_type(result, &status);
  validate_status_ok(&status);
  assert_int_equal(type, OpenPublishValueTypeBoolean);

  openpublish_context_destroy(ctx);
  openpublish_context_destroy(context);
}

static void
test_openpublish_publish_rollback_pre_publish_success(void **state) {
  OpenPublishBasePublish publish = openpublish_publish_new_default();
  publish.pre_publish_fn = publish_should_fail;
  publish.rollback_pre_publish_fn = rollback_should_pass;
  publish.publish_fn = publish_should_fail;
  publish.rollback_publish_fn = rollback_should_fail;
  publish.post_publish_fn = publish_should_fail;
  publish.rollback_post_publish_fn = rollback_should_fail;
  OpenPublishContext *context = openpublish_context_new();
  OpenPublishStatus status;
  openpublish_status_ok(&status);

  openpublish_publish_rollback_pre_publish(&publish, context, &status);
  validate_status_ok(&status);
  openpublish_context_destroy(context);
}

static void test_openpublish_publish_publish_success(void **state) {
  OpenPublishBasePublish publish = openpublish_publish_new_default();
  publish.pre_publish_fn = publish_should_fail;
  publish.rollback_pre_publish_fn = rollback_should_fail;
  publish.publish_fn = publish_should_pass;
  publish.rollback_publish_fn = rollback_should_fail;
  publish.post_publish_fn = publish_should_fail;
  publish.rollback_post_publish_fn = rollback_should_fail;
  OpenPublishContext *context = openpublish_context_new();
  OpenPublishStatus status;
  openpublish_status_ok(&status);

  OpenPublishContext *ctx =
      openpublish_publish_publish(&publish, context, &status);
  validate_status_ok(&status);
  assert_non_null(ctx);
  const OpenPublishValue *result =
      openpublish_context_get(ctx, "test", &status);
  validate_status_ok(&status);
  assert_non_null(result);

  OpenPublishValueType type = openpublish_value_type(result, &status);
  validate_status_ok(&status);
  assert_int_equal(type, OpenPublishValueTypeBoolean);

  openpublish_context_destroy(ctx);
  openpublish_context_destroy(context);
}

static void test_openpublish_publish_rollback_publish_success(void **state) {
  OpenPublishBasePublish publish = openpublish_publish_new_default();
  publish.pre_publish_fn = publish_should_fail;
  publish.rollback_pre_publish_fn = rollback_should_fail;
  publish.publish_fn = publish_should_fail;
  publish.rollback_publish_fn = rollback_should_pass;
  publish.post_publish_fn = publish_should_fail;
  publish.rollback_post_publish_fn = rollback_should_fail;
  OpenPublishContext *context = openpublish_context_new();
  OpenPublishStatus status;
  openpublish_status_ok(&status);

  openpublish_publish_rollback_publish(&publish, context, &status);
  validate_status_ok(&status);

  openpublish_context_destroy(context);
}

static void test_openpublish_publish_post_publish_success(void **state) {
  OpenPublishBasePublish publish = openpublish_publish_new_default();
  publish.pre_publish_fn = publish_should_fail;
  publish.rollback_pre_publish_fn = rollback_should_fail;
  publish.publish_fn = publish_should_fail;
  publish.rollback_publish_fn = rollback_should_fail;
  publish.post_publish_fn = publish_should_pass;
  publish.rollback_post_publish_fn = rollback_should_fail;
  OpenPublishContext *context = openpublish_context_new();
  OpenPublishStatus status;
  openpublish_status_ok(&status);

  OpenPublishContext *ctx =
      openpublish_publish_post_publish(&publish, context, &status);
  validate_status_ok(&status);
  assert_non_null(ctx);
  const OpenPublishValue *result =
      openpublish_context_get(ctx, "test", &status);
  validate_status_ok(&status);
  assert_non_null(result);

  OpenPublishValueType type = openpublish_value_type(result, &status);
  validate_status_ok(&status);
  assert_int_equal(type, OpenPublishValueTypeBoolean);

  openpublish_context_destroy(ctx);
  openpublish_context_destroy(context);
}

static void
test_openpublish_publish_rollback_post_publish_success(void **state) {
  OpenPublishBasePublish publish = openpublish_publish_new_default();
  publish.pre_publish_fn = publish_should_fail;
  publish.rollback_pre_publish_fn = rollback_should_fail;
  publish.publish_fn = publish_should_fail;
  publish.rollback_publish_fn = rollback_should_fail;
  publish.post_publish_fn = publish_should_fail;
  publish.rollback_post_publish_fn = rollback_should_pass;
  OpenPublishContext *context = openpublish_context_new();
  OpenPublishStatus status;
  openpublish_status_ok(&status);

  openpublish_publish_rollback_post_publish(&publish, context, &status);
  validate_status_ok(&status);

  openpublish_context_destroy(context);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_openpublish_publish_new_default_success),
      cmocka_unit_test(test_openpublish_publish_pre_publish_success),
      cmocka_unit_test(test_openpublish_publish_rollback_pre_publish_success),
      cmocka_unit_test(test_openpublish_publish_publish_success),
      cmocka_unit_test(test_openpublish_publish_rollback_publish_success),
      cmocka_unit_test(test_openpublish_publish_post_publish_success),
      cmocka_unit_test(test_openpublish_publish_rollback_post_publish_success),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
