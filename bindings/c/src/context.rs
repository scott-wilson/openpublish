use crate::{openpublish_status_ok, OpenPublishStatus, OpenPublishStringView};
use std::{
    ffi::{c_char, CStr, CString},
    ptr::{null, null_mut},
};

pub struct OpenPublishContext {
    pub inner: base_openpublish::Context,
}

impl From<base_openpublish::Context> for OpenPublishContext {
    fn from(value: base_openpublish::Context) -> Self {
        Self { inner: value }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_context_new() -> *mut OpenPublishContext {
    let inner = base_openpublish::Context::default();
    Box::into_raw(Box::new(OpenPublishContext { inner }))
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_context_destroy(context: *mut OpenPublishContext) {
    if !context.is_null() {
        drop(Box::from_raw(context));
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_context_get(
    context: *const OpenPublishContext,
    key: *const c_char,
    status: *mut crate::OpenPublishStatus,
) -> *const crate::OpenPublishValue {
    openpublish_status_ok(status);

    match context.as_ref() {
        Some(context) => match CStr::from_ptr(key).to_str() {
            Ok(key) => match context.inner.get(key) {
                Some(value) => {
                    value as *const base_openpublish::Value as *const crate::OpenPublishValue
                }
                None => null(),
            },
            Err(_) => {
                if !status.is_null() {
                    *status = OpenPublishStatus::new_error("key is not a valid c-string");
                }

                null()
            }
        },
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("context is null");
            }

            null()
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_context_set(
    context: *mut OpenPublishContext,
    key: *const c_char,
    value: *const crate::OpenPublishValue,
    status: *mut crate::OpenPublishStatus,
) {
    openpublish_status_ok(status);

    match context.as_mut() {
        Some(context) => {
            let key = match key.as_ref() {
                Some(key) => match CStr::from_ptr(key).to_str() {
                    Ok(key) => key,
                    Err(_) => {
                        if !status.is_null() {
                            *status = OpenPublishStatus::new_error("key is not a valid c-string");
                        }

                        return;
                    }
                },
                None => {
                    if !status.is_null() {
                        *status = OpenPublishStatus::new_error("key is null");
                    }

                    return;
                }
            };

            let value = match value.as_ref() {
                Some(value) => &value.value,
                None => {
                    if !status.is_null() {
                        *status = OpenPublishStatus::new_error("value is null");
                    }

                    return;
                }
            };

            context.inner.set(key, value.clone());
        }
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("context is null");
            }
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_context_set_none(
    context: *mut OpenPublishContext,
    key: *const c_char,
    status: *mut crate::OpenPublishStatus,
) {
    openpublish_status_ok(status);

    match context.as_mut() {
        Some(context) => {
            let key = match key.as_ref() {
                Some(key) => match CStr::from_ptr(key).to_str() {
                    Ok(key) => key,
                    Err(_) => {
                        if !status.is_null() {
                            *status = OpenPublishStatus::new_error("key is not a valid c-string");
                        }

                        return;
                    }
                },
                None => {
                    if !status.is_null() {
                        *status = OpenPublishStatus::new_error("key is null");
                    }

                    return;
                }
            };

            context.inner.set(key, base_openpublish::Value::None);
        }
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("context is null");
            }
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_context_set_bool(
    context: *mut OpenPublishContext,
    key: *const c_char,
    value: bool,
    status: *mut crate::OpenPublishStatus,
) {
    openpublish_status_ok(status);

    match context.as_mut() {
        Some(context) => {
            let key = match key.as_ref() {
                Some(key) => match CStr::from_ptr(key).to_str() {
                    Ok(key) => key,
                    Err(_) => {
                        if !status.is_null() {
                            *status = OpenPublishStatus::new_error("key is not a valid c-string");
                        }

                        return;
                    }
                },
                None => {
                    if !status.is_null() {
                        *status = OpenPublishStatus::new_error("key is null");
                    }

                    return;
                }
            };

            context
                .inner
                .set(key, base_openpublish::Value::Boolean(value));
        }
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("context is null");
            }
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_context_set_int(
    context: *mut OpenPublishContext,
    key: *const c_char,
    value: i64,
    status: *mut crate::OpenPublishStatus,
) {
    openpublish_status_ok(status);

    match context.as_mut() {
        Some(context) => {
            let key = match key.as_ref() {
                Some(key) => match CStr::from_ptr(key).to_str() {
                    Ok(key) => key,
                    Err(_) => {
                        if !status.is_null() {
                            *status = OpenPublishStatus::new_error("key is not a valid c-string");
                        }

                        return;
                    }
                },
                None => {
                    if !status.is_null() {
                        *status = OpenPublishStatus::new_error("key is null");
                    }

                    return;
                }
            };

            context
                .inner
                .set(key, base_openpublish::Value::Integer(value));
        }
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("context is null");
            }
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_context_set_float(
    context: *mut OpenPublishContext,
    key: *const c_char,
    value: f64,
    status: *mut crate::OpenPublishStatus,
) {
    openpublish_status_ok(status);

    match context.as_mut() {
        Some(context) => {
            let key = match key.as_ref() {
                Some(key) => match CStr::from_ptr(key).to_str() {
                    Ok(key) => key,
                    Err(_) => {
                        if !status.is_null() {
                            *status = OpenPublishStatus::new_error("key is not a valid c-string");
                        }

                        return;
                    }
                },
                None => {
                    if !status.is_null() {
                        *status = OpenPublishStatus::new_error("key is null");
                    }

                    return;
                }
            };

            context
                .inner
                .set(key, base_openpublish::Value::Float(value));
        }
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("context is null");
            }
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_context_set_string(
    context: *mut OpenPublishContext,
    key: *const c_char,
    value: *const c_char,
    status: *mut crate::OpenPublishStatus,
) {
    openpublish_status_ok(status);

    match context.as_mut() {
        Some(context) => {
            let key = match key.as_ref() {
                Some(key) => match CStr::from_ptr(key).to_str() {
                    Ok(key) => key,
                    Err(_) => {
                        if !status.is_null() {
                            *status = OpenPublishStatus::new_error("key is not a valid c-string");
                        }

                        return;
                    }
                },
                None => {
                    if !status.is_null() {
                        *status = OpenPublishStatus::new_error("key is null");
                    }

                    return;
                }
            };
            let value = match value.as_ref() {
                Some(value) => match CStr::from_ptr(value).to_str() {
                    Ok(value) => value,
                    Err(_) => {
                        if !status.is_null() {
                            *status = OpenPublishStatus::new_error("value is not a valid c-string");
                        }

                        return;
                    }
                },
                None => {
                    if !status.is_null() {
                        *status = OpenPublishStatus::new_error("value is null");
                    }

                    return;
                }
            };

            context
                .inner
                .set(key, base_openpublish::Value::String(value.to_string()));
        }
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("context is null");
            }
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_context_len(
    context: *const OpenPublishContext,
    status: *mut crate::OpenPublishStatus,
) -> usize {
    openpublish_status_ok(status);

    match context.as_ref() {
        Some(context) => context.inner.len(),
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("context is null");
            }

            0
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_context_is_empty(
    context: *const OpenPublishContext,
    status: *mut crate::OpenPublishStatus,
) -> bool {
    openpublish_status_ok(status);

    match context.as_ref() {
        Some(context) => context.inner.is_empty(),
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("context is null");
            }

            true
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_context_clone(
    context: *const OpenPublishContext,
    status: *mut crate::OpenPublishStatus,
) -> *mut OpenPublishContext {
    openpublish_status_ok(status);

    match context.as_ref() {
        Some(context) => Box::into_raw(Box::new(OpenPublishContext {
            inner: context.inner.clone(),
        })),
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("context is null");
            }

            null_mut()
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_context_iter(
    context: *const OpenPublishContext,
    status: *mut crate::OpenPublishStatus,
) -> *mut OpenPublishContextIter {
    openpublish_status_ok(status);

    let context = match context.as_ref() {
        Some(context) => &context.inner,
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("context is null");
            }
            return null_mut();
        }
    };

    let iter = Box::new(OpenPublishContextIter {
        iter: context.iter(),
        key: null_mut(),
        value: null_mut(),
    });
    Box::into_raw(iter)
}

pub struct OpenPublishContextIter {
    iter: base_openpublish::ContextIter<'static>,
    key: *const c_char,
    value: *const crate::OpenPublishValue,
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_context_iter_destroy(iter: *mut OpenPublishContextIter) {
    if !iter.is_null() {
        drop(Box::from_raw(iter));
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_context_iter_next(
    iter: *mut OpenPublishContextIter,
    status: *mut OpenPublishStatus,
) {
    openpublish_status_ok(status);

    match iter.as_mut() {
        Some(iter) => {
            if !iter.key.is_null() {
                drop(CString::from_raw(iter.key as *mut _));
                iter.key = null();
            }

            match iter.iter.next() {
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
                        value as *const base_openpublish::Value as *const crate::OpenPublishValue;
                }
                None => iter.value = null(),
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
pub unsafe extern "C" fn openpublish_context_iter_is_done(
    iter: *mut OpenPublishContextIter,
    status: *mut OpenPublishStatus,
) -> bool {
    openpublish_status_ok(status);

    match iter.as_mut() {
        Some(iter) => iter.value.is_null(),
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("iter is null");
            }

            true
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_context_iter_key(
    iter: *mut OpenPublishContextIter,
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
pub unsafe extern "C" fn openpublish_context_iter_value(
    iter: *mut OpenPublishContextIter,
    status: *mut OpenPublishStatus,
) -> *const crate::OpenPublishValue {
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
