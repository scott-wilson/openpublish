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

/*
----------------------------------------------------------------------------
  Checks
*/

static void test_openpublish_context_set_none_success(void **state) {
  OpenPublishStatus status;

  OpenPublishContext *context = openpublish_context_new();
  assert_non_null(context);

  size_t len;
  len = openpublish_context_len(context, &status);
  validate_status_ok(&status);
  assert_int_equal(len, 0);
  assert_int_equal(openpublish_context_is_empty(context, &status), true);

  openpublish_context_set_none(context, "test", &status);
  validate_status_ok(&status);

  len = openpublish_context_len(context, &status);
  assert_int_equal(len, 1);
  assert_int_equal(openpublish_context_is_empty(context, &status), false);

  const OpenPublishValue *value =
      openpublish_context_get(context, "test", &status);
  assert_non_null(value);
  validate_status_ok(&status);

  assert_int_equal(openpublish_value_type(value, &status),
                   OpenPublishValueTypeNone);
  validate_status_ok(&status);

  openpublish_context_destroy(context);
}

static void test_openpublish_context_set_bool_success(void **state) {
  OpenPublishStatus status;

  OpenPublishContext *context = openpublish_context_new();
  assert_non_null(context);

  size_t len;
  len = openpublish_context_len(context, &status);
  validate_status_ok(&status);
  assert_int_equal(len, 0);
  assert_int_equal(openpublish_context_is_empty(context, &status), true);

  openpublish_context_set_bool(context, "test", true, &status);
  validate_status_ok(&status);

  len = openpublish_context_len(context, &status);
  assert_int_equal(len, 1);
  assert_int_equal(openpublish_context_is_empty(context, &status), false);

  const OpenPublishValue *value =
      openpublish_context_get(context, "test", &status);
  assert_non_null(value);
  validate_status_ok(&status);

  assert_int_equal(openpublish_value_type(value, &status),
                   OpenPublishValueTypeBoolean);
  validate_status_ok(&status);

  assert_true(openpublish_value_bool(value, &status));
  validate_status_ok(&status);

  openpublish_context_destroy(context);
}

static void test_openpublish_context_set_int_success(void **state) {
  OpenPublishStatus status;

  OpenPublishContext *context = openpublish_context_new();
  assert_non_null(context);

  size_t len;
  len = openpublish_context_len(context, &status);
  validate_status_ok(&status);
  assert_int_equal(len, 0);
  assert_int_equal(openpublish_context_is_empty(context, &status), true);

  openpublish_context_set_int(context, "test", 1, &status);
  validate_status_ok(&status);

  len = openpublish_context_len(context, &status);
  assert_int_equal(len, 1);
  assert_int_equal(openpublish_context_is_empty(context, &status), false);

  const OpenPublishValue *value =
      openpublish_context_get(context, "test", &status);
  assert_non_null(value);
  validate_status_ok(&status);

  assert_int_equal(openpublish_value_type(value, &status),
                   OpenPublishValueTypeInteger);
  validate_status_ok(&status);

  assert_int_equal(openpublish_value_int(value, &status), 1);
  validate_status_ok(&status);

  openpublish_context_destroy(context);
}

static void test_openpublish_context_set_float_success(void **state) {
  OpenPublishStatus status;

  OpenPublishContext *context = openpublish_context_new();
  assert_non_null(context);

  size_t len;
  len = openpublish_context_len(context, &status);
  validate_status_ok(&status);
  assert_int_equal(len, 0);
  assert_int_equal(openpublish_context_is_empty(context, &status), true);

  openpublish_context_set_float(context, "test", 1.0, &status);
  validate_status_ok(&status);

  len = openpublish_context_len(context, &status);
  assert_int_equal(len, 1);
  assert_int_equal(openpublish_context_is_empty(context, &status), false);

  const OpenPublishValue *value =
      openpublish_context_get(context, "test", &status);
  assert_non_null(value);
  validate_status_ok(&status);

  assert_int_equal(openpublish_value_type(value, &status),
                   OpenPublishValueTypeFloat);
  validate_status_ok(&status);

  assert_float_equal(openpublish_value_float(value, &status), 1.0, DBL_EPSILON);
  validate_status_ok(&status);

  openpublish_context_destroy(context);
}

static void test_openpublish_context_set_string_success(void **state) {
  OpenPublishStatus status;

  OpenPublishContext *context = openpublish_context_new();
  assert_non_null(context);

  size_t len;
  len = openpublish_context_len(context, &status);
  validate_status_ok(&status);
  assert_int_equal(len, 0);
  assert_int_equal(openpublish_context_is_empty(context, &status), true);

  openpublish_context_set_string(context, "test", "test", &status);
  validate_status_ok(&status);

  len = openpublish_context_len(context, &status);
  assert_int_equal(len, 1);
  assert_int_equal(openpublish_context_is_empty(context, &status), false);

  const OpenPublishValue *value =
      openpublish_context_get(context, "test", &status);
  assert_non_null(value);
  validate_status_ok(&status);

  assert_int_equal(openpublish_value_type(value, &status),
                   OpenPublishValueTypeString);
  validate_status_ok(&status);

  OpenPublishString result = openpublish_value_string(value, &status);
  validate_status_ok(&status);
  assert_string_equal(result.string, "test");
  validate_status_ok(&status);

  openpublish_string_destroy(&result);

  openpublish_context_destroy(context);
}

static void test_openpublish_context_set_success(void **state) {
  OpenPublishStatus status;

  OpenPublishContext *context = openpublish_context_new();
  assert_non_null(context);

  size_t len;
  len = openpublish_context_len(context, &status);
  validate_status_ok(&status);
  assert_int_equal(len, 0);
  assert_int_equal(openpublish_context_is_empty(context, &status), true);

  OpenPublishValue *value = openpublish_value_new_none();
  assert_non_null(value);

  openpublish_context_set(context, "test", value, &status);
  validate_status_ok(&status);
  openpublish_value_destroy(value);

  len = openpublish_context_len(context, &status);
  assert_int_equal(len, 1);
  assert_int_equal(openpublish_context_is_empty(context, &status), false);

  const OpenPublishValue *value_out =
      openpublish_context_get(context, "test", &status);
  assert_non_null(value_out);
  validate_status_ok(&status);

  assert_int_equal(openpublish_value_type(value_out, &status),
                   OpenPublishValueTypeNone);
  validate_status_ok(&status);

  openpublish_context_destroy(context);
}

static void test_openpublish_context_clone_success(void **state) {
  OpenPublishStatus status;

  OpenPublishContext *context = openpublish_context_new();
  assert_non_null(context);

  size_t len;
  len = openpublish_context_len(context, &status);
  validate_status_ok(&status);
  assert_int_equal(len, 0);
  assert_int_equal(openpublish_context_is_empty(context, &status), true);

  openpublish_context_set_none(context, "test", &status);
  validate_status_ok(&status);

  len = openpublish_context_len(context, &status);
  assert_int_equal(len, 1);
  assert_int_equal(openpublish_context_is_empty(context, &status), false);

  OpenPublishContext *cloned_context =
      openpublish_context_clone(context, &status);

  const OpenPublishValue *value =
      openpublish_context_get(cloned_context, "test", &status);
  assert_non_null(value);
  validate_status_ok(&status);

  assert_int_equal(openpublish_value_type(value, &status),
                   OpenPublishValueTypeNone);
  validate_status_ok(&status);

  openpublish_context_destroy(context);
  openpublish_context_destroy(cloned_context);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_openpublish_context_set_none_success),
      cmocka_unit_test(test_openpublish_context_set_bool_success),
      cmocka_unit_test(test_openpublish_context_set_int_success),
      cmocka_unit_test(test_openpublish_context_set_float_success),
      cmocka_unit_test(test_openpublish_context_set_string_success),
      cmocka_unit_test(test_openpublish_context_set_success),
      cmocka_unit_test(test_openpublish_context_clone_success),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
