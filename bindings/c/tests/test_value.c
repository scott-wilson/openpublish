#include <float.h>
#include <math.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <cmocka.h>

#include "cpublish.h"
#include "test_utils.h"

/*
----------------------------------------------------------------------------
  Checks
*/
static void test_cpublish_value_clone_success(void **state) {
  CPublishStatus status;
  CPublishValue *value = cpublish_value_new_none();
  CPublishValue *cloned = cpublish_value_clone(value, &status);
  validate_status_ok(&status);

  CPublishValueType expected_type = cpublish_value_type(value, &status);
  validate_status_ok(&status);
  CPublishValueType result_type = cpublish_value_type(cloned, &status);
  validate_status_ok(&status);

  assert_int_equal(result_type, expected_type);

  cpublish_value_destroy(value);
  cpublish_value_destroy(cloned);
  cpublish_status_destroy(&status);
}

static void test_cpublish_value_new_none_success(void **state) {
  CPublishStatus status;
  CPublishValue *value = cpublish_value_new_none();
  assert_non_null(value);

  assert_int_equal(cpublish_value_type(value, &status), CPublishValueTypeNone);
  validate_status_ok(&status);

  cpublish_value_destroy(value);
  cpublish_status_destroy(&status);
}

void helper_test_cpublish_value_new_bool_success(bool v,
                                                 CPublishStatus *status) {
  CPublishValue *value = cpublish_value_new_bool(v);
  assert_non_null(value);

  assert_int_equal(cpublish_value_type(value, status),
                   CPublishValueTypeBoolean);
  validate_status_ok(status);

  bool result = cpublish_value_bool(value, status);
  validate_status_ok(status);
  assert_int_equal(result, v);

  cpublish_value_destroy(value);
}

static void test_cpublish_value_new_bool_success(void **state) {
  CPublishStatus status;

  helper_test_cpublish_value_new_bool_success(true, &status);
  helper_test_cpublish_value_new_bool_success(false, &status);

  cpublish_status_destroy(&status);
}

void helper_test_cpublish_value_new_int_success(int64_t v,
                                                CPublishStatus *status) {
  CPublishValue *value = cpublish_value_new_int(v);
  assert_non_null(value);

  assert_int_equal(cpublish_value_type(value, status),
                   CPublishValueTypeInteger);
  validate_status_ok(status);

  int64_t result = cpublish_value_int(value, status);
  validate_status_ok(status);
  assert_int_equal(result, v);

  cpublish_value_destroy(value);
}

static void test_cpublish_value_new_int_success(void **state) {
  CPublishStatus status;

  helper_test_cpublish_value_new_int_success(INT64_MIN, &status);
  helper_test_cpublish_value_new_int_success(-10, &status);
  helper_test_cpublish_value_new_int_success(-1, &status);
  helper_test_cpublish_value_new_int_success(0, &status);
  helper_test_cpublish_value_new_int_success(1, &status);
  helper_test_cpublish_value_new_int_success(10, &status);
  helper_test_cpublish_value_new_int_success(INT64_MAX, &status);

  cpublish_status_destroy(&status);
}

void helper_test_cpublish_value_new_float_success(double v,
                                                  CPublishStatus *status) {
  CPublishValue *value = cpublish_value_new_float(v);
  assert_non_null(value);

  assert_int_equal(cpublish_value_type(value, status), CPublishValueTypeFloat);
  validate_status_ok(status);

  double result = cpublish_value_float(value, status);
  validate_status_ok(status);

  if (isnan(v)) {
    assert_true(isnan(result));
  } else {
    assert_double_equal(result, v, DBL_EPSILON);
  }

  cpublish_value_destroy(value);
}

static void test_cpublish_value_new_float_success(void **state) {
  CPublishStatus status;

  helper_test_cpublish_value_new_float_success(DBL_MIN, &status);
  helper_test_cpublish_value_new_float_success(-10.0, &status);
  helper_test_cpublish_value_new_float_success(-1.0, &status);
  helper_test_cpublish_value_new_float_success(0.0, &status);
  helper_test_cpublish_value_new_float_success(1.0, &status);
  helper_test_cpublish_value_new_float_success(10.0, &status);
  helper_test_cpublish_value_new_float_success(DBL_MAX, &status);
  helper_test_cpublish_value_new_float_success(NAN, &status);
  helper_test_cpublish_value_new_float_success(INFINITY, &status);
  helper_test_cpublish_value_new_float_success(-INFINITY, &status);

  cpublish_status_destroy(&status);
}

void helper_test_cpublish_value_new_string_success(const char *v,
                                                   CPublishStatus *status) {
  CPublishValue *value = cpublish_value_new_string(v);
  assert_non_null(value);

  assert_int_equal(cpublish_value_type(value, status), CPublishValueTypeString);
  validate_status_ok(status);

  CPublishString result = cpublish_value_string(value, status);
  validate_status_ok(status);
  assert_string_equal(result.string, v);

  cpublish_string_destroy(&result);
  cpublish_value_destroy(value);
}

static void test_cpublish_value_new_string_success(void **state) {
  CPublishStatus status;

  helper_test_cpublish_value_new_string_success("", &status);
  helper_test_cpublish_value_new_string_success("test", &status);
  helper_test_cpublish_value_new_string_success(
      "test test test test test test test test test", &status);
  helper_test_cpublish_value_new_string_success("12345", &status);

  cpublish_status_destroy(&status);
}

static void test_cpublish_value_new_array_success(void **state) {
  CPublishStatus status;

  CPublishValue *value = cpublish_value_new_array();
  assert_non_null(value);

  assert_int_equal(cpublish_value_type(value, &status), CPublishValueTypeArray);
  validate_status_ok(&status);

  // No items
  assert_int_equal(cpublish_value_array_len(value, &status), 0);
  validate_status_ok(&status);

  // Added item
  CPublishValue *item = cpublish_value_new_none();
  cpublish_value_array_push(value, item, &status);
  cpublish_value_destroy(item);
  validate_status_ok(&status);
  assert_int_equal(cpublish_value_array_len(value, &status), 1);
  validate_status_ok(&status);
  cpublish_value_array_get(value, 0, &status);
  validate_status_ok(&status);
  CPublishValueIterArray *iter = cpublish_value_array_iter(value, &status);
  validate_status_ok(&status);
  size_t count = 0;

  while (!cpublish_value_iter_array_is_done(iter, &status)) {
    count++;
    const CPublishValue *item = cpublish_value_iter_array_value(iter, &status);
    validate_status_ok(&status);
    assert_int_equal(cpublish_value_type(item, &status), CPublishValueTypeNone);
    validate_status_ok(&status);
    cpublish_value_iter_array_next(iter, &status);
    validate_status_ok(&status);
  }

  assert_int_equal(count, 1);

  cpublish_value_iter_array_destroy(iter);

  cpublish_value_destroy(value);
  cpublish_status_destroy(&status);
}

static void test_cpublish_value_new_array_with_capacity_success(void **state) {
  CPublishStatus status;

  CPublishValue *value = cpublish_value_new_array_with_capacity(1);
  assert_non_null(value);

  assert_int_equal(cpublish_value_type(value, &status), CPublishValueTypeArray);
  validate_status_ok(&status);

  // No items
  assert_int_equal(cpublish_value_array_len(value, &status), 0);
  validate_status_ok(&status);

  // Added item
  CPublishValue *item = cpublish_value_new_none();
  cpublish_value_array_push(value, item, &status);
  cpublish_value_destroy(item);
  validate_status_ok(&status);
  assert_int_equal(cpublish_value_array_len(value, &status), 1);
  validate_status_ok(&status);
  cpublish_value_array_get(value, 0, &status);
  validate_status_ok(&status);
  CPublishValueIterArray *iter = cpublish_value_array_iter(value, &status);
  validate_status_ok(&status);
  size_t count = 0;

  while (!cpublish_value_iter_array_is_done(iter, &status)) {
    count++;
    const CPublishValue *item = cpublish_value_iter_array_value(iter, &status);
    validate_status_ok(&status);
    assert_int_equal(cpublish_value_type(item, &status), CPublishValueTypeNone);
    validate_status_ok(&status);
    cpublish_value_iter_array_next(iter, &status);
    validate_status_ok(&status);
  }

  assert_int_equal(count, 1);

  cpublish_value_iter_array_destroy(iter);

  cpublish_value_destroy(value);
  cpublish_status_destroy(&status);
}

static void test_cpublish_value_new_object_success(void **state) {
  CPublishStatus status;

  CPublishValue *value = cpublish_value_new_object();
  assert_non_null(value);

  assert_int_equal(cpublish_value_type(value, &status),
                   CPublishValueTypeObject);
  validate_status_ok(&status);

  // No items
  assert_int_equal(cpublish_value_object_len(value, &status), 0);
  validate_status_ok(&status);

  // Added item
  CPublishValue *item = cpublish_value_new_none();
  cpublish_value_object_insert(value, "test", item, &status);
  cpublish_value_destroy(item);
  validate_status_ok(&status);
  assert_int_equal(cpublish_value_object_len(value, &status), 1);
  validate_status_ok(&status);
  cpublish_value_object_get(value, "test", &status);
  validate_status_ok(&status);
  CPublishValueIterObject *iter = cpublish_value_object_iter(value, &status);
  validate_status_ok(&status);
  size_t count = 0;

  while (!cpublish_value_iter_object_is_done(iter, &status)) {
    count++;
    const CPublishValue *item = cpublish_value_iter_object_value(iter, &status);
    validate_status_ok(&status);
    assert_int_equal(cpublish_value_type(item, &status), CPublishValueTypeNone);
    validate_status_ok(&status);
    cpublish_value_iter_object_next(iter, &status);
    validate_status_ok(&status);
  }

  assert_int_equal(count, 1);

  cpublish_value_iter_object_destroy(iter);

  cpublish_value_destroy(value);
  cpublish_status_destroy(&status);
}

static void test_cpublish_value_new_object_with_capacity_success(void **state) {
  CPublishStatus status;

  CPublishValue *value = cpublish_value_new_object_with_capacity(1);
  assert_non_null(value);

  assert_int_equal(cpublish_value_type(value, &status),
                   CPublishValueTypeObject);
  validate_status_ok(&status);

  // No items
  assert_int_equal(cpublish_value_object_len(value, &status), 0);
  validate_status_ok(&status);

  // Added item
  CPublishValue *item = cpublish_value_new_none();
  cpublish_value_object_insert(value, "test", item, &status);
  cpublish_value_destroy(item);
  validate_status_ok(&status);
  assert_int_equal(cpublish_value_object_len(value, &status), 1);
  validate_status_ok(&status);
  cpublish_value_object_get(value, "test", &status);
  validate_status_ok(&status);
  CPublishValueIterObject *iter = cpublish_value_object_iter(value, &status);
  validate_status_ok(&status);
  size_t count = 0;

  while (!cpublish_value_iter_object_is_done(iter, &status)) {
    count++;
    const CPublishValue *item = cpublish_value_iter_object_value(iter, &status);
    validate_status_ok(&status);
    assert_int_equal(cpublish_value_type(item, &status), CPublishValueTypeNone);
    validate_status_ok(&status);
    cpublish_value_iter_object_next(iter, &status);
    validate_status_ok(&status);
  }

  assert_int_equal(count, 1);

  cpublish_value_iter_object_destroy(iter);

  cpublish_value_destroy(value);
  cpublish_status_destroy(&status);
}

static void test_cpublish_value_comparison(void **state) {
  CPublishStatus status;

  CPublishValue *none = cpublish_value_new_none();
  CPublishValue *bool_true = cpublish_value_new_bool(true);
  CPublishValue *bool_false = cpublish_value_new_bool(false);
  CPublishValue *int_0 = cpublish_value_new_int(0);
  CPublishValue *int_1 = cpublish_value_new_int(1);
  CPublishValue *float_0 = cpublish_value_new_float(0.0);
  CPublishValue *float_1 = cpublish_value_new_float(1.0);
  CPublishValue *string_empty = cpublish_value_new_string("");
  CPublishValue *string_test = cpublish_value_new_string("test");
  CPublishValue *array_empty = cpublish_value_new_array();
  CPublishValue *array_1 = cpublish_value_new_array();
  CPublishValue *object_empty = cpublish_value_new_object();
  CPublishValue *object_1 = cpublish_value_new_object();

  cpublish_value_array_push(array_1, none, &status);
  validate_status_ok(&status);
  cpublish_value_object_insert(object_1, "test", none, &status);
  validate_status_ok(&status);

  CPublishValue *values[] = {none,        bool_true,   bool_false, int_0,
                             int_1,       float_0,     float_1,    string_empty,
                             string_test, array_empty, array_1,    object_empty,
                             object_1};

  for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
    for (size_t j = 0; j < sizeof(values) / sizeof(values[0]); j++) {
      if (i == j) {
        assert_true(cpublish_value_eq(values[i], values[j], &status));
        validate_status_ok(&status);
      } else {
        assert_false(cpublish_value_eq(values[i], values[j], &status));
        validate_status_ok(&status);
      }
    }
  }

  cpublish_value_destroy(none);
  cpublish_value_destroy(bool_true);
  cpublish_value_destroy(bool_false);
  cpublish_value_destroy(int_0);
  cpublish_value_destroy(int_1);
  cpublish_value_destroy(float_0);
  cpublish_value_destroy(float_1);
  cpublish_value_destroy(string_empty);
  cpublish_value_destroy(string_test);
  cpublish_value_destroy(array_empty);
  cpublish_value_destroy(array_1);
  cpublish_value_destroy(object_empty);
  cpublish_value_destroy(object_1);
  cpublish_status_destroy(&status);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_cpublish_value_new_none_success),
      cmocka_unit_test(test_cpublish_value_new_bool_success),
      cmocka_unit_test(test_cpublish_value_new_int_success),
      cmocka_unit_test(test_cpublish_value_new_float_success),
      cmocka_unit_test(test_cpublish_value_new_string_success),
      cmocka_unit_test(test_cpublish_value_new_array_success),
      cmocka_unit_test(test_cpublish_value_new_array_with_capacity_success),
      cmocka_unit_test(test_cpublish_value_new_object_success),
      cmocka_unit_test(test_cpublish_value_new_object_with_capacity_success),
      cmocka_unit_test(test_cpublish_value_comparison),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
