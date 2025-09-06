use std::{ffi::CStr, ptr::null_mut};

use crate::{
    openpublish_context_destroy, openpublish_status_ok, OpenPublishContext, OpenPublishStatus,
};

#[repr(C)]
pub struct OpenPublishBasePublish {
    pub pre_publish_fn: unsafe extern "C" fn(
        publish: *const Self,
        context: *const OpenPublishContext,
        status: *mut OpenPublishStatus,
    ) -> *mut OpenPublishContext,
    pub rollback_pre_publish_fn: unsafe extern "C" fn(
        publish: *const Self,
        context: *const OpenPublishContext,
        status: *mut OpenPublishStatus,
    ),
    pub publish_fn: unsafe extern "C" fn(
        publish: *const Self,
        context: *const OpenPublishContext,
        status: *mut OpenPublishStatus,
    ) -> *mut OpenPublishContext,
    pub rollback_publish_fn: unsafe extern "C" fn(
        publish: *const Self,
        context: *const OpenPublishContext,
        status: *mut OpenPublishStatus,
    ),
    pub post_publish_fn: unsafe extern "C" fn(
        publish: *const Self,
        context: *const OpenPublishContext,
        status: *mut OpenPublishStatus,
    ) -> *mut OpenPublishContext,
    pub rollback_post_publish_fn: unsafe extern "C" fn(
        publish: *const Self,
        context: *const OpenPublishContext,
        status: *mut OpenPublishStatus,
    ),
}

#[async_trait::async_trait]
impl base_openpublish::Publish for OpenPublishBasePublish {
    async fn pre_publish<'a>(
        &self,
        context: &'a base_openpublish::Context,
    ) -> Result<Option<base_openpublish::Context>, base_openpublish::Error> {
        let c_context = context as *const base_openpublish::Context as *const OpenPublishContext;
        let mut status = OpenPublishStatus::new_ok();

        let result = unsafe { openpublish_publish_pre_publish(self, c_context, &mut status) };

        let out_result = match status.status {
            crate::OpenPublishStatusType::OpenPublishStatusTypeOk => {
                match unsafe { result.as_ref() } {
                    Some(c_context) => Ok(Some(c_context.inner.clone())),
                    None => Ok(None),
                }
            }
            crate::OpenPublishStatusType::OpenPublishStatusTypeError => {
                let message = unsafe { CStr::from_ptr(status.message) };
                Err(base_openpublish::Error::new_publish(
                    message.to_string_lossy(),
                    None,
                ))
            }
        };

        if !result.is_null() {
            unsafe {
                openpublish_context_destroy(result);
            }
        }

        out_result
    }

    async fn rollback_pre_publish(
        &self,
        context: &base_openpublish::Context,
    ) -> Result<(), base_openpublish::Error> {
        let c_context = context as *const base_openpublish::Context as *const OpenPublishContext;
        let mut status = OpenPublishStatus::new_ok();

        unsafe { openpublish_publish_rollback_pre_publish(self, c_context, &mut status) };

        match status.status {
            crate::OpenPublishStatusType::OpenPublishStatusTypeOk => Ok(()),
            crate::OpenPublishStatusType::OpenPublishStatusTypeError => {
                let message = unsafe { CStr::from_ptr(status.message) };
                Err(base_openpublish::Error::new_publish(
                    message.to_string_lossy(),
                    None,
                ))
            }
        }
    }

    async fn publish<'a>(
        &self,
        context: &'a base_openpublish::Context,
    ) -> Result<Option<base_openpublish::Context>, base_openpublish::Error> {
        let c_context = context as *const base_openpublish::Context as *const OpenPublishContext;
        let mut status = OpenPublishStatus::new_ok();

        let result = unsafe { openpublish_publish_publish(self, c_context, &mut status) };

        let out_result = match status.status {
            crate::OpenPublishStatusType::OpenPublishStatusTypeOk => {
                match unsafe { result.as_ref() } {
                    Some(c_context) => Ok(Some(c_context.inner.clone())),
                    None => Ok(None),
                }
            }
            crate::OpenPublishStatusType::OpenPublishStatusTypeError => {
                let message = unsafe { CStr::from_ptr(status.message) };
                Err(base_openpublish::Error::new_publish(
                    message.to_string_lossy(),
                    None,
                ))
            }
        };

        if !result.is_null() {
            unsafe {
                openpublish_context_destroy(result);
            }
        }

        out_result
    }

    async fn rollback_publish(
        &self,
        context: &base_openpublish::Context,
    ) -> Result<(), base_openpublish::Error> {
        let c_context = context as *const base_openpublish::Context as *const OpenPublishContext;
        let mut status = OpenPublishStatus::new_ok();

        unsafe { openpublish_publish_rollback_publish(self, c_context, &mut status) };

        match status.status {
            crate::OpenPublishStatusType::OpenPublishStatusTypeOk => Ok(()),
            crate::OpenPublishStatusType::OpenPublishStatusTypeError => {
                let message = unsafe { CStr::from_ptr(status.message) };
                Err(base_openpublish::Error::new_publish(
                    message.to_string_lossy(),
                    None,
                ))
            }
        }
    }

    async fn post_publish<'a>(
        &self,
        context: &'a base_openpublish::Context,
    ) -> Result<Option<base_openpublish::Context>, base_openpublish::Error> {
        let c_context = context as *const base_openpublish::Context as *const OpenPublishContext;
        let mut status = OpenPublishStatus::new_ok();

        let result = unsafe { openpublish_publish_post_publish(self, c_context, &mut status) };

        let out_result = match status.status {
            crate::OpenPublishStatusType::OpenPublishStatusTypeOk => {
                match unsafe { result.as_ref() } {
                    Some(c_context) => {
                        println!("{:?}", c_context.inner.len());
                        Ok(Some(c_context.inner.clone()))
                    }
                    None => Ok(None),
                }
            }
            crate::OpenPublishStatusType::OpenPublishStatusTypeError => {
                let message = unsafe { CStr::from_ptr(status.message) };
                Err(base_openpublish::Error::new_publish(
                    message.to_string_lossy(),
                    None,
                ))
            }
        };

        if !result.is_null() {
            unsafe {
                openpublish_context_destroy(result);
            }
        }

        out_result
    }

    async fn rollback_post_publish(
        &self,
        context: &base_openpublish::Context,
    ) -> Result<(), base_openpublish::Error> {
        let c_context = context as *const base_openpublish::Context as *const OpenPublishContext;
        let mut status = OpenPublishStatus::new_ok();

        unsafe { openpublish_publish_rollback_post_publish(self, c_context, &mut status) };

        match status.status {
            crate::OpenPublishStatusType::OpenPublishStatusTypeOk => Ok(()),
            crate::OpenPublishStatusType::OpenPublishStatusTypeError => {
                let message = unsafe { CStr::from_ptr(status.message) };
                Err(base_openpublish::Error::new_publish(
                    message.to_string_lossy(),
                    None,
                ))
            }
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_publish_default_publish(
    publish: *const OpenPublishBasePublish,
    context: *const OpenPublishContext,
    status: *mut OpenPublishStatus,
) -> *mut OpenPublishContext {
    openpublish_status_ok(status);

    if publish.is_null() {
        if !status.is_null() {
            *status = OpenPublishStatus::new_error("publish is null");
        }
        return null_mut();
    }

    if context.is_null() {
        if !status.is_null() {
            *status = OpenPublishStatus::new_error("context is null");
        }
        return null_mut();
    }

    if !status.is_null() {
        *status = OpenPublishStatus::new_ok();
    }

    null_mut()
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_publish_default_error_publish(
    #[allow(unused_variables)] publish: *const OpenPublishBasePublish,
    #[allow(unused_variables)] context: *const OpenPublishContext,
    status: *mut OpenPublishStatus,
) -> *mut OpenPublishContext {
    *status = OpenPublishStatus::new_error("openpublishBasePublish->publish_fn is not set");
    null_mut()
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_publish_default_rollback_publish(
    publish: *const OpenPublishBasePublish,
    context: *const OpenPublishContext,
    status: *mut OpenPublishStatus,
) {
    openpublish_status_ok(status);

    if publish.is_null() {
        if !status.is_null() {
            *status = OpenPublishStatus::new_error("publish is null");
        }
        return;
    }

    if context.is_null() {
        if !status.is_null() {
            *status = OpenPublishStatus::new_error("context is null");
        }
        return;
    }

    if !status.is_null() {
        *status = OpenPublishStatus::new_ok();
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_publish_new_default() -> OpenPublishBasePublish {
    OpenPublishBasePublish {
        pre_publish_fn: openpublish_publish_default_publish,
        rollback_pre_publish_fn: openpublish_publish_default_rollback_publish,
        publish_fn: openpublish_publish_default_error_publish,
        rollback_publish_fn: openpublish_publish_default_rollback_publish,
        post_publish_fn: openpublish_publish_default_publish,
        rollback_post_publish_fn: openpublish_publish_default_rollback_publish,
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_publish_pre_publish(
    publish: *const OpenPublishBasePublish,
    context: *const OpenPublishContext,
    status: *mut OpenPublishStatus,
) -> *mut OpenPublishContext {
    openpublish_status_ok(status);

    match publish.as_ref() {
        Some(publish) => (publish.pre_publish_fn)(publish, context, status),
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("publish is null");
            }
            std::ptr::null_mut()
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_publish_rollback_pre_publish(
    publish: *const OpenPublishBasePublish,
    context: *const OpenPublishContext,
    status: *mut OpenPublishStatus,
) {
    openpublish_status_ok(status);

    match publish.as_ref() {
        Some(publish) => (publish.rollback_pre_publish_fn)(publish, context, status),
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("publish is null");
            }
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_publish_publish(
    publish: *const OpenPublishBasePublish,
    context: *const OpenPublishContext,
    status: *mut OpenPublishStatus,
) -> *mut OpenPublishContext {
    openpublish_status_ok(status);

    match publish.as_ref() {
        Some(publish) => (publish.publish_fn)(publish, context, status),
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("publish is null");
            }
            std::ptr::null_mut()
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_publish_rollback_publish(
    publish: *const OpenPublishBasePublish,
    context: *const OpenPublishContext,
    status: *mut OpenPublishStatus,
) {
    openpublish_status_ok(status);

    match publish.as_ref() {
        Some(publish) => (publish.rollback_publish_fn)(publish, context, status),
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("publish is null");
            }
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_publish_post_publish(
    publish: *const OpenPublishBasePublish,
    context: *const OpenPublishContext,
    status: *mut OpenPublishStatus,
) -> *mut OpenPublishContext {
    openpublish_status_ok(status);

    match publish.as_ref() {
        Some(publish) => (publish.post_publish_fn)(publish, context, status),
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("publish is null");
            }
            std::ptr::null_mut()
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_publish_rollback_post_publish(
    publish: *const OpenPublishBasePublish,
    context: *const OpenPublishContext,
    status: *mut OpenPublishStatus,
) {
    openpublish_status_ok(status);

    match publish.as_ref() {
        Some(publish) => (publish.rollback_post_publish_fn)(publish, context, status),
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("publish is null");
            }
        }
    }
}
