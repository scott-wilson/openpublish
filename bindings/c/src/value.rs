use crate::{openpublish_status_ok, OpenPublishStatus, OpenPublishString, OpenPublishStringView};
use std::{
    ffi::{c_char, CStr, CString},
    ptr::{null, null_mut},
};

#[derive(Clone, Debug, PartialEq)]
pub struct OpenPublishValue {
    pub value: base_openpublish::Value,
}

impl From<base_openpublish::Value> for OpenPublishValue {
    fn from(value: base_openpublish::Value) -> Self {
        Self { value }
    }
}

#[derive(Debug, PartialEq)]
#[repr(C)]
pub enum OpenPublishValueType {
    OpenPublishValueTypeNone,
    OpenPublishValueTypeBoolean,
    OpenPublishValueTypeInteger,
    OpenPublishValueTypeFloat,
    OpenPublishValueTypeString,
    OpenPublishValueTypeArray,
    OpenPublishValueTypeObject,
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_clone(
    value: *const OpenPublishValue,
    status: *mut crate::OpenPublishStatus,
) -> *mut OpenPublishValue {
    openpublish_status_ok(status);

    match value.as_ref() {
        Some(value) => Box::into_raw(Box::new(value.clone())),
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("value is null");
            }

            null_mut()
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_new_none() -> *mut OpenPublishValue {
    let value = base_openpublish::Value::None;
    Box::into_raw(Box::new(OpenPublishValue { value }))
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_new_bool(value: bool) -> *mut OpenPublishValue {
    let value = base_openpublish::Value::Boolean(value);
    Box::into_raw(Box::new(OpenPublishValue { value }))
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_new_int(value: i64) -> *mut OpenPublishValue {
    let value = base_openpublish::Value::Integer(value);
    Box::into_raw(Box::new(OpenPublishValue { value }))
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_new_float(value: f64) -> *mut OpenPublishValue {
    let value = base_openpublish::Value::Float(value);
    Box::into_raw(Box::new(OpenPublishValue { value }))
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_new_string(
    value: *const c_char,
) -> *mut OpenPublishValue {
    let value =
        base_openpublish::Value::String(CStr::from_ptr(value).to_string_lossy().into_owned());
    Box::into_raw(Box::new(OpenPublishValue { value }))
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_new_array() -> *mut OpenPublishValue {
    let value = base_openpublish::Value::Array(Vec::new());
    Box::into_raw(Box::new(OpenPublishValue { value }))
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_new_array_with_capacity(
    capacity: usize,
) -> *mut OpenPublishValue {
    let value = base_openpublish::Value::Array(Vec::with_capacity(capacity));
    Box::into_raw(Box::new(OpenPublishValue { value }))
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_new_object() -> *mut OpenPublishValue {
    let value = base_openpublish::Value::Object(std::collections::HashMap::new());
    Box::into_raw(Box::new(OpenPublishValue { value }))
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_new_object_with_capacity(
    capacity: usize,
) -> *mut OpenPublishValue {
    let value = base_openpublish::Value::Object(std::collections::HashMap::with_capacity(capacity));
    Box::into_raw(Box::new(OpenPublishValue { value }))
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_destroy(value: *mut OpenPublishValue) {
    if !value.is_null() {
        drop(Box::from_raw(value));
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_type(
    value: *const OpenPublishValue,
    status: *mut OpenPublishStatus,
) -> OpenPublishValueType {
    openpublish_status_ok(status);

    if value.is_null() {
        if !status.is_null() {
            *status = OpenPublishStatus::new_error("value is null");
        }
        return OpenPublishValueType::OpenPublishValueTypeNone;
    }

    match (*value).value {
        base_openpublish::Value::None => OpenPublishValueType::OpenPublishValueTypeNone,
        base_openpublish::Value::Boolean(_) => OpenPublishValueType::OpenPublishValueTypeBoolean,
        base_openpublish::Value::Integer(_) => OpenPublishValueType::OpenPublishValueTypeInteger,
        base_openpublish::Value::Float(_) => OpenPublishValueType::OpenPublishValueTypeFloat,
        base_openpublish::Value::String(_) => OpenPublishValueType::OpenPublishValueTypeString,
        base_openpublish::Value::Array(_) => OpenPublishValueType::OpenPublishValueTypeArray,
        base_openpublish::Value::Object(_) => OpenPublishValueType::OpenPublishValueTypeObject,
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_eq(
    value: *const OpenPublishValue,
    other: *const OpenPublishValue,
    status: *mut OpenPublishStatus,
) -> bool {
    openpublish_status_ok(status);

    let value = match value.as_ref() {
        Some(value) => value,
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("value is null");
            }
            return false;
        }
    };

    let other = match other.as_ref() {
        Some(other) => other,
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("other is null");
            }
            return false;
        }
    };

    value == other
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_bool(
    value: *const OpenPublishValue,
    status: *mut OpenPublishStatus,
) -> bool {
    openpublish_status_ok(status);

    match value.as_ref() {
        Some(value) => match value.value {
            base_openpublish::Value::Boolean(value) => value,
            _ => {
                if !status.is_null() {
                    *status = OpenPublishStatus::new_error("value is not a boolean");
                }
                false
            }
        },
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("value is null");
            }
            false
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_int(
    value: *const OpenPublishValue,
    status: *mut OpenPublishStatus,
) -> i64 {
    openpublish_status_ok(status);

    match value.as_ref() {
        Some(value) => match value.value {
            base_openpublish::Value::Integer(value) => value,
            _ => {
                if !status.is_null() {
                    *status = OpenPublishStatus::new_error("value is not an integer");
                }
                0
            }
        },
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("value is null");
            }
            0
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_float(
    value: *const OpenPublishValue,
    status: *mut OpenPublishStatus,
) -> f64 {
    openpublish_status_ok(status);

    match value.as_ref() {
        Some(value) => match value.value {
            base_openpublish::Value::Float(value) => value,
            _ => {
                if !status.is_null() {
                    *status = OpenPublishStatus::new_error("value is not a float");
                }
                0.0
            }
        },
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("value is null");
            }
            0.0
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_string(
    value: *const OpenPublishValue,
    status: *mut OpenPublishStatus,
) -> OpenPublishString {
    openpublish_status_ok(status);

    match value.as_ref() {
        Some(value) => match &value.value {
            base_openpublish::Value::String(value) => OpenPublishString::new(value),
            _ => {
                if !status.is_null() {
                    *status = OpenPublishStatus::new_error("value is not a string");
                }
                OpenPublishString::new("")
            }
        },
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("value is null");
            }
            OpenPublishString::new("")
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_array_len(
    value: *const OpenPublishValue,
    status: *mut OpenPublishStatus,
) -> usize {
    openpublish_status_ok(status);

    match value.as_ref() {
        Some(value) => match &value.value {
            base_openpublish::Value::Array(value) => value.len(),
            _ => {
                if !status.is_null() {
                    *status = OpenPublishStatus::new_error("value is not an array");
                }
                0
            }
        },
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("value is null");
            }
            0
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_array_get(
    value: *const OpenPublishValue,
    index: usize,
    status: *mut OpenPublishStatus,
) -> *const OpenPublishValue {
    openpublish_status_ok(status);

    match value.as_ref() {
        Some(value) => match &value.value {
            base_openpublish::Value::Array(value) => match value.get(index) {
                Some(value) => value as *const base_openpublish::Value as *const OpenPublishValue,
                None => {
                    if !status.is_null() {
                        *status = OpenPublishStatus::new_error("index out of bounds");
                    }
                    null()
                }
            },
            _ => {
                if !status.is_null() {
                    *status = OpenPublishStatus::new_error("value is not an array");
                }
                null()
            }
        },
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("value is null");
            }
            null()
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_array_push(
    value: *mut OpenPublishValue,
    item: *const OpenPublishValue,
    status: *mut OpenPublishStatus,
) {
    openpublish_status_ok(status);

    if item.is_null() {
        if !status.is_null() {
            *status = OpenPublishStatus::new_error("item is null");
        }
        return;
    }

    match value.as_mut() {
        Some(value) => match &mut value.value {
            base_openpublish::Value::Array(value) => value.push((*item).value.clone()),
            _ => {
                if !status.is_null() {
                    *status = OpenPublishStatus::new_error("value is not an array");
                }
            }
        },
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("value is null");
            }
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_array_iter(
    value: *const OpenPublishValue,
    status: *mut OpenPublishStatus,
) -> *mut OpenPublishValueIterArray {
    openpublish_status_ok(status);

    let value = match value.as_ref() {
        Some(value) => value,
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("value is null");
            }

            return null_mut();
        }
    };

    let iter = match &value.value {
        base_openpublish::Value::Array(value) => {
            let iter = Box::new(OpenPublishValueIterArray {
                iter: if !value.is_empty() {
                    Some(value.iter())
                } else {
                    None
                },
                value: null_mut(),
            });
            Box::into_raw(iter)
        }
        _ => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("value is not an array");
            }
            null_mut()
        }
    };

    openpublish_value_iter_array_next(iter, status);

    match status.as_ref() {
        Some(_) => iter,
        None => {
            openpublish_value_iter_array_destroy(iter);
            null_mut()
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_object_len(
    value: *const OpenPublishValue,
    status: *mut OpenPublishStatus,
) -> usize {
    openpublish_status_ok(status);

    match value.as_ref() {
        Some(value) => match &value.value {
            base_openpublish::Value::Object(value) => value.len(),
            _ => {
                if !status.is_null() {
                    *status = OpenPublishStatus::new_error("value is not an object");
                }
                0
            }
        },
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("value is null");
            }
            0
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_object_get(
    value: *const OpenPublishValue,
    key: *const c_char,
    status: *mut OpenPublishStatus,
) -> *const OpenPublishValue {
    openpublish_status_ok(status);

    if key.is_null() {
        if !status.is_null() {
            *status = OpenPublishStatus::new_error("key is null");
        }
        return null();
    }

    let key = CStr::from_ptr(key).to_string_lossy();

    match value.as_ref() {
        Some(value) => match &value.value {
            base_openpublish::Value::Object(value) => match value.get(key.as_ref()) {
                Some(value) => value as *const base_openpublish::Value as *const OpenPublishValue,
                None => {
                    if !status.is_null() {
                        *status = OpenPublishStatus::new_error("value is not in object");
                    }
                    null()
                }
            },
            _ => {
                if !status.is_null() {
                    *status = OpenPublishStatus::new_error("value is not in object");
                }
                null()
            }
        },
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("value is null");
            }
            null()
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_object_insert(
    value: *mut OpenPublishValue,
    key: *const c_char,
    item: *const OpenPublishValue,
    status: *mut OpenPublishStatus,
) {
    openpublish_status_ok(status);

    if key.is_null() {
        if !status.is_null() {
            *status = OpenPublishStatus::new_error("key is null");
        }
        return;
    }

    if item.is_null() {
        if !status.is_null() {
            *status = OpenPublishStatus::new_error("item is null");
        }
        return;
    }

    match value.as_mut() {
        Some(value) => match &mut value.value {
            base_openpublish::Value::Object(value) => {
                let key = CStr::from_ptr(key).to_string_lossy().to_string();
                value.insert(key, (*item).value.clone());
            }
            _ => {
                if !status.is_null() {
                    *status = OpenPublishStatus::new_error("value is not an array");
                }
            }
        },
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("value is null");
            }
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_object_iter(
    value: *const OpenPublishValue,
    status: *mut OpenPublishStatus,
) -> *mut OpenPublishValueIterObject {
    openpublish_status_ok(status);

    let value = match value.as_ref() {
        Some(value) => value,
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("value is null");
            }

            return null_mut();
        }
    };

    let iter = match &value.value {
        base_openpublish::Value::Object(value) => {
            let iter = Box::new(OpenPublishValueIterObject {
                iter: if !value.is_empty() {
                    Some(value.iter())
                } else {
                    None
                },
                key: null_mut(),
                value: null_mut(),
            });
            Box::into_raw(iter)
        }
        _ => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("value is not an object");
            }
            null_mut()
        }
    };

    openpublish_value_iter_object_next(iter, status);

    match status.as_ref() {
        Some(_) => iter,
        None => {
            openpublish_value_iter_object_destroy(iter);
            null_mut()
        }
    }
}

pub struct OpenPublishValueIterArray {
    iter: Option<std::slice::Iter<'static, base_openpublish::Value>>,
    value: *const OpenPublishValue,
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_iter_array_destroy(
    iter: *mut OpenPublishValueIterArray,
) {
    if !iter.is_null() {
        drop(Box::from_raw(iter));
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_iter_array_next(
    iter: *mut OpenPublishValueIterArray,
    status: *mut OpenPublishStatus,
) {
    openpublish_status_ok(status);

    match iter.as_mut() {
        Some(iter) => match &mut iter.iter {
            Some(iter_inner) => match iter_inner.next() {
                Some(value) => {
                    iter.value = value as *const base_openpublish::Value as *const OpenPublishValue
                }
                None => {
                    iter.value = null();
                    iter.iter = None;
                }
            },
            None => {
                iter.value = null();
                iter.iter = None;
            }
        },
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("iter is null");
            }
        }
    };
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_iter_array_is_done(
    iter: *mut OpenPublishValueIterArray,
    status: *mut OpenPublishStatus,
) -> bool {
    openpublish_status_ok(status);

    match iter.as_mut() {
        Some(iter) => iter.value.is_null() && iter.iter.is_none(),
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("iter is null");
            }

            true
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_iter_array_value(
    iter: *mut OpenPublishValueIterArray,
    status: *mut OpenPublishStatus,
) -> *const OpenPublishValue {
    openpublish_status_ok(status);

    match iter.as_mut() {
        Some(iter) => iter.value,
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("iter is null");
            }

            null()
        }
    }
}

pub struct OpenPublishValueIterObject {
    iter: Option<std::collections::hash_map::Iter<'static, String, base_openpublish::Value>>,
    key: *const c_char,
    value: *const OpenPublishValue,
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_iter_object_destroy(
    iter: *mut OpenPublishValueIterObject,
) {
    if !iter.is_null() {
        drop(Box::from_raw(iter));
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_iter_object_next(
    iter: *mut OpenPublishValueIterObject,
    status: *mut OpenPublishStatus,
) {
    openpublish_status_ok(status);

    match iter.as_mut() {
        Some(iter) => {
            if !iter.key.is_null() {
                drop(CString::from_raw(iter.key as *mut _));
                iter.key = null();
            }

            match &mut iter.iter {
                Some(iter_inner) => match iter_inner.next() {
                    Some((key, value)) => {
                        iter.key = match CString::new::<&str>(key.as_ref()) {
                            Ok(key) => key.into_raw(),
                            Err(_) => {
                                if !status.is_null() {
                                    *status =
                                        OpenPublishStatus::new_error("key is not a valid c-string");
                                }
                                return;
                            }
                        };
                        iter.value =
                            value as *const base_openpublish::Value as *const OpenPublishValue
                    }
                    None => {
                        iter.value = null();
                        iter.iter = None;
                    }
                },
                None => {
                    iter.value = null();
                    iter.iter = None;
                }
            }
        }
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("iter is null");
            }
        }
    };
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_iter_object_is_done(
    iter: *mut OpenPublishValueIterObject,
    status: *mut OpenPublishStatus,
) -> bool {
    openpublish_status_ok(status);

    match iter.as_mut() {
        Some(iter) => iter.value.is_null() && iter.iter.is_none(),
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("iter is null");
            }

            true
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_iter_object_key(
    iter: *mut OpenPublishValueIterObject,
    status: *mut OpenPublishStatus,
) -> OpenPublishStringView {
    openpublish_status_ok(status);

    match iter.as_mut() {
        Some(iter) => OpenPublishStringView { string: iter.key },
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("iter is null");
            }

            OpenPublishStringView { string: null() }
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_value_iter_object_value(
    iter: *mut OpenPublishValueIterObject,
    status: *mut OpenPublishStatus,
) -> *const OpenPublishValue {
    openpublish_status_ok(status);

    match iter.as_mut() {
        Some(iter) => iter.value,
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("iter is null");
            }

            null()
        }
    }
}
