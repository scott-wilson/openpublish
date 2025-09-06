#ifndef copenpublish_h
#define copenpublish_h

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>


typedef enum OpenPublishStatusType {
  OpenPublishStatusTypeOk,
  OpenPublishStatusTypeError,
} OpenPublishStatusType;

typedef enum OpenPublishValueType {
  OpenPublishValueTypeNone,
  OpenPublishValueTypeBoolean,
  OpenPublishValueTypeInteger,
  OpenPublishValueTypeFloat,
  OpenPublishValueTypeString,
  OpenPublishValueTypeArray,
  OpenPublishValueTypeObject,
} OpenPublishValueType;

typedef struct OpenPublishContext OpenPublishContext;

typedef struct OpenPublishContextIter OpenPublishContextIter;

typedef struct OpenPublishValue OpenPublishValue;

typedef struct OpenPublishValueIterArray OpenPublishValueIterArray;

typedef struct OpenPublishValueIterObject OpenPublishValueIterObject;

typedef struct OpenPublishStatus {
  enum OpenPublishStatusType status;
  const char *message;
} OpenPublishStatus;

/**
 * The OpenPublishStringView creates a borrowed pointer to a C style string.
 *
 * # Safety
 *
 * The pointer must not outlive the container that owns the string. Also, the
 * pointer should not be null, but that is not a strict requirement.
 */
typedef struct OpenPublishStringView {
  /**
   * The borrowed pointer to a string.
   *
   * # Safety
   *
   * The string must not outlive the container that owns it.
   */
  const char *string;
} OpenPublishStringView;

typedef struct OpenPublishBasePublish {
  struct OpenPublishContext *(*pre_publish_fn)(const struct OpenPublishBasePublish *publish,
                                               const struct OpenPublishContext *context,
                                               struct OpenPublishStatus *status);
  void (*rollback_pre_publish_fn)(const struct OpenPublishBasePublish *publish,
                                  const struct OpenPublishContext *context,
                                  struct OpenPublishStatus *status);
  struct OpenPublishContext *(*publish_fn)(const struct OpenPublishBasePublish *publish,
                                           const struct OpenPublishContext *context,
                                           struct OpenPublishStatus *status);
  void (*rollback_publish_fn)(const struct OpenPublishBasePublish *publish,
                              const struct OpenPublishContext *context,
                              struct OpenPublishStatus *status);
  struct OpenPublishContext *(*post_publish_fn)(const struct OpenPublishBasePublish *publish,
                                                const struct OpenPublishContext *context,
                                                struct OpenPublishStatus *status);
  void (*rollback_post_publish_fn)(const struct OpenPublishBasePublish *publish,
                                   const struct OpenPublishContext *context,
                                   struct OpenPublishStatus *status);
} OpenPublishBasePublish;

/**
 * The OpenPublishString contains an owned pointer to a C style string.
 *
 * # Safety
 *
 * The pointer to the string must be destroyed with `openpublish_string_destroy`
 * once it is no longer needed. Also, the pointer must not be modified at all
 * by any functions not exposed by the validation library.
 *
 * Internally, if a OpenPublishString is created, the system will create a copy of
 * the string being pointed to.
 */
typedef struct OpenPublishString {
  /**
   * The owned pointer to a string.
   *
   * # Safety
   *
   * This should not be modified at all outside of the validation library.
   * Also, it should only be destroyed with `openpublish_string_destroy`.
   */
  char *string;
  /**
   * Destroy the owned data.
   *
   * # Safety
   *
   * The destroy function should be called once at most.
   *
   * The destroy function should handle if the string pointer is null.
   */
  void (*destroy_fn)(struct OpenPublishString*);
} OpenPublishString;

struct OpenPublishContext *openpublish_context_clone(const struct OpenPublishContext *context,
                                                     struct OpenPublishStatus *status);

void openpublish_context_destroy(struct OpenPublishContext *context);

const struct OpenPublishValue *openpublish_context_get(const struct OpenPublishContext *context,
                                                       const char *key,
                                                       struct OpenPublishStatus *status);

bool openpublish_context_is_empty(const struct OpenPublishContext *context,
                                  struct OpenPublishStatus *status);

struct OpenPublishContextIter *openpublish_context_iter(const struct OpenPublishContext *context,
                                                        struct OpenPublishStatus *status);

void openpublish_context_iter_destroy(struct OpenPublishContextIter *iter);

bool openpublish_context_iter_is_done(struct OpenPublishContextIter *iter,
                                      struct OpenPublishStatus *status);

struct OpenPublishStringView openpublish_context_iter_key(struct OpenPublishContextIter *iter,
                                                          struct OpenPublishStatus *status);

void openpublish_context_iter_next(struct OpenPublishContextIter *iter,
                                   struct OpenPublishStatus *status);

const struct OpenPublishValue *openpublish_context_iter_value(struct OpenPublishContextIter *iter,
                                                              struct OpenPublishStatus *status);

size_t openpublish_context_len(const struct OpenPublishContext *context,
                               struct OpenPublishStatus *status);

struct OpenPublishContext *openpublish_context_new(void);

void openpublish_context_set(struct OpenPublishContext *context,
                             const char *key,
                             const struct OpenPublishValue *value,
                             struct OpenPublishStatus *status);

void openpublish_context_set_bool(struct OpenPublishContext *context,
                                  const char *key,
                                  bool value,
                                  struct OpenPublishStatus *status);

void openpublish_context_set_float(struct OpenPublishContext *context,
                                   const char *key,
                                   double value,
                                   struct OpenPublishStatus *status);

void openpublish_context_set_int(struct OpenPublishContext *context,
                                 const char *key,
                                 int64_t value,
                                 struct OpenPublishStatus *status);

void openpublish_context_set_none(struct OpenPublishContext *context,
                                  const char *key,
                                  struct OpenPublishStatus *status);

void openpublish_context_set_string(struct OpenPublishContext *context,
                                    const char *key,
                                    const char *value,
                                    struct OpenPublishStatus *status);

struct OpenPublishContext *openpublish_publish_default_error_publish(const struct OpenPublishBasePublish *publish,
                                                                     const struct OpenPublishContext *context,
                                                                     struct OpenPublishStatus *status);

struct OpenPublishContext *openpublish_publish_default_publish(const struct OpenPublishBasePublish *publish,
                                                               const struct OpenPublishContext *context,
                                                               struct OpenPublishStatus *status);

void openpublish_publish_default_rollback_publish(const struct OpenPublishBasePublish *publish,
                                                  const struct OpenPublishContext *context,
                                                  struct OpenPublishStatus *status);

struct OpenPublishBasePublish openpublish_publish_new_default(void);

struct OpenPublishContext *openpublish_publish_post_publish(const struct OpenPublishBasePublish *publish,
                                                            const struct OpenPublishContext *context,
                                                            struct OpenPublishStatus *status);

struct OpenPublishContext *openpublish_publish_pre_publish(const struct OpenPublishBasePublish *publish,
                                                           const struct OpenPublishContext *context,
                                                           struct OpenPublishStatus *status);

struct OpenPublishContext *openpublish_publish_publish(const struct OpenPublishBasePublish *publish,
                                                       const struct OpenPublishContext *context,
                                                       struct OpenPublishStatus *status);

void openpublish_publish_rollback_post_publish(const struct OpenPublishBasePublish *publish,
                                               const struct OpenPublishContext *context,
                                               struct OpenPublishStatus *status);

void openpublish_publish_rollback_pre_publish(const struct OpenPublishBasePublish *publish,
                                              const struct OpenPublishContext *context,
                                              struct OpenPublishStatus *status);

void openpublish_publish_rollback_publish(const struct OpenPublishBasePublish *publish,
                                          const struct OpenPublishContext *context,
                                          struct OpenPublishStatus *status);

struct OpenPublishContext *openpublish_run(const struct OpenPublishBasePublish *publish,
                                           struct OpenPublishStatus *status);

void openpublish_status_destroy(struct OpenPublishStatus *status);

void openpublish_status_error(struct OpenPublishStatus *status, const char *message);

void openpublish_status_ok(struct OpenPublishStatus *status);

/**
 * Destroy a string pointer.
 *
 * # Safety
 *
 * The pointer must not be null, and must not already have been destroyed (AKA:
 * double free). Once the destroy function is called, all pointers to the
 * string are invalid.
 */
void openpublish_string_destroy(struct OpenPublishString *string);

const struct OpenPublishValue *openpublish_value_array_get(const struct OpenPublishValue *value,
                                                           size_t index,
                                                           struct OpenPublishStatus *status);

struct OpenPublishValueIterArray *openpublish_value_array_iter(const struct OpenPublishValue *value,
                                                               struct OpenPublishStatus *status);

size_t openpublish_value_array_len(const struct OpenPublishValue *value,
                                   struct OpenPublishStatus *status);

void openpublish_value_array_push(struct OpenPublishValue *value,
                                  const struct OpenPublishValue *item,
                                  struct OpenPublishStatus *status);

bool openpublish_value_bool(const struct OpenPublishValue *value, struct OpenPublishStatus *status);

struct OpenPublishValue *openpublish_value_clone(const struct OpenPublishValue *value,
                                                 struct OpenPublishStatus *status);

void openpublish_value_destroy(struct OpenPublishValue *value);

bool openpublish_value_eq(const struct OpenPublishValue *value,
                          const struct OpenPublishValue *other,
                          struct OpenPublishStatus *status);

double openpublish_value_float(const struct OpenPublishValue *value,
                               struct OpenPublishStatus *status);

int64_t openpublish_value_int(const struct OpenPublishValue *value,
                              struct OpenPublishStatus *status);

void openpublish_value_iter_array_destroy(struct OpenPublishValueIterArray *iter);

bool openpublish_value_iter_array_is_done(struct OpenPublishValueIterArray *iter,
                                          struct OpenPublishStatus *status);

void openpublish_value_iter_array_next(struct OpenPublishValueIterArray *iter,
                                       struct OpenPublishStatus *status);

const struct OpenPublishValue *openpublish_value_iter_array_value(struct OpenPublishValueIterArray *iter,
                                                                  struct OpenPublishStatus *status);

void openpublish_value_iter_object_destroy(struct OpenPublishValueIterObject *iter);

bool openpublish_value_iter_object_is_done(struct OpenPublishValueIterObject *iter,
                                           struct OpenPublishStatus *status);

struct OpenPublishStringView openpublish_value_iter_object_key(struct OpenPublishValueIterObject *iter,
                                                               struct OpenPublishStatus *status);

void openpublish_value_iter_object_next(struct OpenPublishValueIterObject *iter,
                                        struct OpenPublishStatus *status);

const struct OpenPublishValue *openpublish_value_iter_object_value(struct OpenPublishValueIterObject *iter,
                                                                   struct OpenPublishStatus *status);

struct OpenPublishValue *openpublish_value_new_array(void);

struct OpenPublishValue *openpublish_value_new_array_with_capacity(size_t capacity);

struct OpenPublishValue *openpublish_value_new_bool(bool value);

struct OpenPublishValue *openpublish_value_new_float(double value);

struct OpenPublishValue *openpublish_value_new_int(int64_t value);

struct OpenPublishValue *openpublish_value_new_none(void);

struct OpenPublishValue *openpublish_value_new_object(void);

struct OpenPublishValue *openpublish_value_new_object_with_capacity(size_t capacity);

struct OpenPublishValue *openpublish_value_new_string(const char *value);

const struct OpenPublishValue *openpublish_value_object_get(const struct OpenPublishValue *value,
                                                            const char *key,
                                                            struct OpenPublishStatus *status);

void openpublish_value_object_insert(struct OpenPublishValue *value,
                                     const char *key,
                                     const struct OpenPublishValue *item,
                                     struct OpenPublishStatus *status);

struct OpenPublishValueIterObject *openpublish_value_object_iter(const struct OpenPublishValue *value,
                                                                 struct OpenPublishStatus *status);

size_t openpublish_value_object_len(const struct OpenPublishValue *value,
                                    struct OpenPublishStatus *status);

struct OpenPublishString openpublish_value_string(const struct OpenPublishValue *value,
                                                  struct OpenPublishStatus *status);

enum OpenPublishValueType openpublish_value_type(const struct OpenPublishValue *value,
                                                 struct OpenPublishStatus *status);

#endif  /* copenpublish_h */
