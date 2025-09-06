#include <float.h>
#include <math.h>
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
static void test_openpublish_value_clone_success(void **state) {
  OpenPublishStatus status;
  OpenPublishValue *value = openpublish_value_new_none();
  OpenPublishValue *cloned = openpublish_value_clone(value, &status);
  validate_status_ok(&status);

  OpenPublishValueType expected_type = openpublish_value_type(value, &status);
  validate_status_ok(&status);
  OpenPublishValueType result_type = openpublish_value_type(cloned, &status);
  validate_status_ok(&status);

  assert_int_equal(result_type, expected_type);

  openpublish_value_destroy(value);
  openpublish_value_destroy(cloned);
  openpublish_status_destroy(&status);
}

static void test_openpublish_value_new_none_success(void **state) {
  OpenPublishStatus status;
  OpenPublishValue *value = openpublish_value_new_none();
  assert_non_null(value);

  assert_int_equal(openpublish_value_type(value, &status),
                   OpenPublishValueTypeNone);
  validate_status_ok(&status);

  openpublish_value_destroy(value);
  openpublish_status_destroy(&status);
}

void helper_test_openpublish_value_new_bool_success(bool v,
                                                    OpenPublishStatus *status) {
  OpenPublishValue *value = openpublish_value_new_bool(v);
  assert_non_null(value);

  assert_int_equal(openpublish_value_type(value, status),
                   OpenPublishValueTypeBoolean);
  validate_status_ok(status);

  bool result = openpublish_value_bool(value, status);
  validate_status_ok(status);
  assert_int_equal(result, v);

  openpublish_value_destroy(value);
}

static void test_openpublish_value_new_bool_success(void **state) {
  OpenPublishStatus status;

  helper_test_openpublish_value_new_bool_success(true, &status);
  helper_test_openpublish_value_new_bool_success(false, &status);

  openpublish_status_destroy(&status);
}

void helper_test_openpublish_value_new_int_success(int64_t v,
                                                   OpenPublishStatus *status) {
  OpenPublishValue *value = openpublish_value_new_int(v);
  assert_non_null(value);

  assert_int_equal(openpublish_value_type(value, status),
                   OpenPublishValueTypeInteger);
  validate_status_ok(status);

  int64_t result = openpublish_value_int(value, status);
  validate_status_ok(status);
  assert_int_equal(result, v);

  openpublish_value_destroy(value);
}

static void test_openpublish_value_new_int_success(void **state) {
  OpenPublishStatus status;

  helper_test_openpublish_value_new_int_success(INT64_MIN, &status);
  helper_test_openpublish_value_new_int_success(-10, &status);
  helper_test_openpublish_value_new_int_success(-1, &status);
  helper_test_openpublish_value_new_int_success(0, &status);
  helper_test_openpublish_value_new_int_success(1, &status);
  helper_test_openpublish_value_new_int_success(10, &status);
  helper_test_openpublish_value_new_int_success(INT64_MAX, &status);

  openpublish_status_destroy(&status);
}

void helper_test_openpublish_value_new_float_success(
    double v, OpenPublishStatus *status) {
  OpenPublishValue *value = openpublish_value_new_float(v);
  assert_non_null(value);

  assert_int_equal(openpublish_value_type(value, status),
                   OpenPublishValueTypeFloat);
  validate_status_ok(status);

  double result = openpublish_value_float(value, status);
  validate_status_ok(status);

  if (isnan(v)) {
    assert_true(isnan(result));
  } else {
    assert_double_equal(result, v, DBL_EPSILON);
  }

  openpublish_value_destroy(value);
}

static void test_openpublish_value_new_float_success(void **state) {
  OpenPublishStatus status;

  helper_test_openpublish_value_new_float_success(DBL_MIN, &status);
  helper_test_openpublish_value_new_float_success(-10.0, &status);
  helper_test_openpublish_value_new_float_success(-1.0, &status);
  helper_test_openpublish_value_new_float_success(0.0, &status);
  helper_test_openpublish_value_new_float_success(1.0, &status);
  helper_test_openpublish_value_new_float_success(10.0, &status);
  helper_test_openpublish_value_new_float_success(DBL_MAX, &status);
  helper_test_openpublish_value_new_float_success(NAN, &status);
  helper_test_openpublish_value_new_float_success(INFINITY, &status);
  helper_test_openpublish_value_new_float_success(-INFINITY, &status);

  openpublish_status_destroy(&status);
}

void helper_test_openpublish_value_new_string_success(
    const char *v, OpenPublishStatus *status) {
  OpenPublishValue *value = openpublish_value_new_string(v);
  assert_non_null(value);

  assert_int_equal(openpublish_value_type(value, status),
                   OpenPublishValueTypeString);
  validate_status_ok(status);

  OpenPublishString result = openpublish_value_string(value, status);
  validate_status_ok(status);
  assert_string_equal(result.string, v);

  openpublish_string_destroy(&result);
  openpublish_value_destroy(value);
}

static void test_openpublish_value_new_string_success(void **state) {
  OpenPublishStatus status;

  helper_test_openpublish_value_new_string_success("", &status);
  helper_test_openpublish_value_new_string_success("test", &status);
  helper_test_openpublish_value_new_string_success(
      "test test test test test test test test test", &status);
  helper_test_openpublish_value_new_string_success("12345", &status);

  openpublish_status_destroy(&status);
}

static void test_openpublish_value_new_array_success(void **state) {
  OpenPublishStatus status;

  OpenPublishValue *value = openpublish_value_new_array();
  assert_non_null(value);

  assert_int_equal(openpublish_value_type(value, &status),
                   OpenPublishValueTypeArray);
  validate_status_ok(&status);

  // No items
  assert_int_equal(openpublish_value_array_len(value, &status), 0);
  validate_status_ok(&status);

  // Added item
  OpenPublishValue *item = openpublish_value_new_none();
  openpublish_value_array_push(value, item, &status);
  openpublish_value_destroy(item);
  validate_status_ok(&status);
  assert_int_equal(openpublish_value_array_len(value, &status), 1);
  validate_status_ok(&status);
  openpublish_value_array_get(value, 0, &status);
  validate_status_ok(&status);
  OpenPublishValueIterArray *iter =
      openpublish_value_array_iter(value, &status);
  validate_status_ok(&status);
  size_t count = 0;

  while (!openpublish_value_iter_array_is_done(iter, &status)) {
    count++;
    const OpenPublishValue *item =
        openpublish_value_iter_array_value(iter, &status);
    validate_status_ok(&status);
    assert_int_equal(openpublish_value_type(item, &status),
                     OpenPublishValueTypeNone);
    validate_status_ok(&status);
    openpublish_value_iter_array_next(iter, &status);
    validate_status_ok(&status);
  }

  assert_int_equal(count, 1);

  openpublish_value_iter_array_destroy(iter);

  openpublish_value_destroy(value);
  openpublish_status_destroy(&status);
}

static void
test_openpublish_value_new_array_with_capacity_success(void **state) {
  OpenPublishStatus status;

  OpenPublishValue *value = openpublish_value_new_array_with_capacity(1);
  assert_non_null(value);

  assert_int_equal(openpublish_value_type(value, &status),
                   OpenPublishValueTypeArray);
  validate_status_ok(&status);

  // No items
  assert_int_equal(openpublish_value_array_len(value, &status), 0);
  validate_status_ok(&status);

  // Added item
  OpenPublishValue *item = openpublish_value_new_none();
  openpublish_value_array_push(value, item, &status);
  openpublish_value_destroy(item);
  validate_status_ok(&status);
  assert_int_equal(openpublish_value_array_len(value, &status), 1);
  validate_status_ok(&status);
  openpublish_value_array_get(value, 0, &status);
  validate_status_ok(&status);
  OpenPublishValueIterArray *iter =
      openpublish_value_array_iter(value, &status);
  validate_status_ok(&status);
  size_t count = 0;

  while (!openpublish_value_iter_array_is_done(iter, &status)) {
    count++;
    const OpenPublishValue *item =
        openpublish_value_iter_array_value(iter, &status);
    validate_status_ok(&status);
    assert_int_equal(openpublish_value_type(item, &status),
                     OpenPublishValueTypeNone);
    validate_status_ok(&status);
    openpublish_value_iter_array_next(iter, &status);
    validate_status_ok(&status);
  }

  assert_int_equal(count, 1);

  openpublish_value_iter_array_destroy(iter);

  openpublish_value_destroy(value);
  openpublish_status_destroy(&status);
}

static void test_openpublish_value_new_object_success(void **state) {
  OpenPublishStatus status;

  OpenPublishValue *value = openpublish_value_new_object();
  assert_non_null(value);

  assert_int_equal(openpublish_value_type(value, &status),
                   OpenPublishValueTypeObject);
  validate_status_ok(&status);

  // No items
  assert_int_equal(openpublish_value_object_len(value, &status), 0);
  validate_status_ok(&status);

  // Added item
  OpenPublishValue *item = openpublish_value_new_none();
  openpublish_value_object_insert(value, "test", item, &status);
  openpublish_value_destroy(item);
  validate_status_ok(&status);
  assert_int_equal(openpublish_value_object_len(value, &status), 1);
  validate_status_ok(&status);
  openpublish_value_object_get(value, "test", &status);
  validate_status_ok(&status);
  OpenPublishValueIterObject *iter =
      openpublish_value_object_iter(value, &status);
  validate_status_ok(&status);
  size_t count = 0;

  while (!openpublish_value_iter_object_is_done(iter, &status)) {
    count++;
    const OpenPublishValue *item =
        openpublish_value_iter_object_value(iter, &status);
    validate_status_ok(&status);
    assert_int_equal(openpublish_value_type(item, &status),
                     OpenPublishValueTypeNone);
    validate_status_ok(&status);
    openpublish_value_iter_object_next(iter, &status);
    validate_status_ok(&status);
  }

  assert_int_equal(count, 1);

  openpublish_value_iter_object_destroy(iter);

  openpublish_value_destroy(value);
  openpublish_status_destroy(&status);
}

static void
test_openpublish_value_new_object_with_capacity_success(void **state) {
  OpenPublishStatus status;

  OpenPublishValue *value = openpublish_value_new_object_with_capacity(1);
  assert_non_null(value);

  assert_int_equal(openpublish_value_type(value, &status),
                   OpenPublishValueTypeObject);
  validate_status_ok(&status);

  // No items
  assert_int_equal(openpublish_value_object_len(value, &status), 0);
  validate_status_ok(&status);

  // Added item
  OpenPublishValue *item = openpublish_value_new_none();
  openpublish_value_object_insert(value, "test", item, &status);
  openpublish_value_destroy(item);
  validate_status_ok(&status);
  assert_int_equal(openpublish_value_object_len(value, &status), 1);
  validate_status_ok(&status);
  openpublish_value_object_get(value, "test", &status);
  validate_status_ok(&status);
  OpenPublishValueIterObject *iter =
      openpublish_value_object_iter(value, &status);
  validate_status_ok(&status);
  size_t count = 0;

  while (!openpublish_value_iter_object_is_done(iter, &status)) {
    count++;
    const OpenPublishValue *item =
        openpublish_value_iter_object_value(iter, &status);
    validate_status_ok(&status);
    assert_int_equal(openpublish_value_type(item, &status),
                     OpenPublishValueTypeNone);
    validate_status_ok(&status);
    openpublish_value_iter_object_next(iter, &status);
    validate_status_ok(&status);
  }

  assert_int_equal(count, 1);

  openpublish_value_iter_object_destroy(iter);

  openpublish_value_destroy(value);
  openpublish_status_destroy(&status);
}

static void test_openpublish_value_comparison(void **state) {
  OpenPublishStatus status;

  OpenPublishValue *none = openpublish_value_new_none();
  OpenPublishValue *bool_true = openpublish_value_new_bool(true);
  OpenPublishValue *bool_false = openpublish_value_new_bool(false);
  OpenPublishValue *int_0 = openpublish_value_new_int(0);
  OpenPublishValue *int_1 = openpublish_value_new_int(1);
  OpenPublishValue *float_0 = openpublish_value_new_float(0.0);
  OpenPublishValue *float_1 = openpublish_value_new_float(1.0);
  OpenPublishValue *string_empty = openpublish_value_new_string("");
  OpenPublishValue *string_test = openpublish_value_new_string("test");
  OpenPublishValue *array_empty = openpublish_value_new_array();
  OpenPublishValue *array_1 = openpublish_value_new_array();
  OpenPublishValue *object_empty = openpublish_value_new_object();
  OpenPublishValue *object_1 = openpublish_value_new_object();

  openpublish_value_array_push(array_1, none, &status);
  validate_status_ok(&status);
  openpublish_value_object_insert(object_1, "test", none, &status);
  validate_status_ok(&status);

  OpenPublishValue *values[] = {
      none,    bool_true,    bool_false,   int_0,       int_1,
      float_0, float_1,      string_empty, string_test, array_empty,
      array_1, object_empty, object_1};

  for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
    for (size_t j = 0; j < sizeof(values) / sizeof(values[0]); j++) {
      if (i == j) {
        assert_true(openpublish_value_eq(values[i], values[j], &status));
        validate_status_ok(&status);
      } else {
        assert_false(openpublish_value_eq(values[i], values[j], &status));
        validate_status_ok(&status);
      }
    }
  }

  openpublish_value_destroy(none);
  openpublish_value_destroy(bool_true);
  openpublish_value_destroy(bool_false);
  openpublish_value_destroy(int_0);
  openpublish_value_destroy(int_1);
  openpublish_value_destroy(float_0);
  openpublish_value_destroy(float_1);
  openpublish_value_destroy(string_empty);
  openpublish_value_destroy(string_test);
  openpublish_value_destroy(array_empty);
  openpublish_value_destroy(array_1);
  openpublish_value_destroy(object_empty);
  openpublish_value_destroy(object_1);
  openpublish_status_destroy(&status);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_openpublish_value_new_none_success),
      cmocka_unit_test(test_openpublish_value_new_bool_success),
      cmocka_unit_test(test_openpublish_value_new_int_success),
      cmocka_unit_test(test_openpublish_value_new_float_success),
      cmocka_unit_test(test_openpublish_value_new_string_success),
      cmocka_unit_test(test_openpublish_value_new_array_success),
      cmocka_unit_test(test_openpublish_value_new_array_with_capacity_success),
      cmocka_unit_test(test_openpublish_value_new_object_success),
      cmocka_unit_test(test_openpublish_value_new_object_with_capacity_success),
      cmocka_unit_test(test_openpublish_value_comparison),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
