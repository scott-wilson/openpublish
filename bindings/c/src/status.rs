use std::{
    ffi::{c_char, CStr, CString},
    ptr::null,
};

#[derive(Debug, PartialEq)]
#[repr(C)]
pub enum OpenPublishStatusType {
    OpenPublishStatusTypeOk,
    OpenPublishStatusTypeError,
}

#[repr(C)]
pub struct OpenPublishStatus {
    pub status: OpenPublishStatusType,
    pub message: *const c_char,
}

impl Drop for OpenPublishStatus {
    fn drop(&mut self) {
        unsafe {
            if !self.message.is_null() {
                drop(CString::from_raw(self.message as *mut _));
            }
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_status_ok(status: *mut OpenPublishStatus) {
    if let Some(status) = status.as_mut() {
        // We need to replace the current status value with a new one without
        // dropping the old one, otherwise we may get a segfault.
        let old_value = std::mem::replace(status, OpenPublishStatus::new_ok());
        std::mem::forget(old_value);
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_status_error(
    status: *mut OpenPublishStatus,
    message: *const c_char,
) {
    if let Some(status) = status.as_mut() {
        let message = CStr::from_ptr(message).to_string_lossy();
        let old_value = std::mem::replace(status, OpenPublishStatus::new_error(message));
        std::mem::forget(old_value);
    }
}

#[no_mangle]
pub unsafe extern "C" fn openpublish_status_destroy(status: *mut OpenPublishStatus) {
    if !status.is_null() {
        status.drop_in_place();
    }
}

impl OpenPublishStatus {
    pub fn new_ok() -> Self {
        Self {
            status: OpenPublishStatusType::OpenPublishStatusTypeOk,
            message: null(),
        }
    }

    pub fn new_error<T: AsRef<str>>(message: T) -> Self {
        let message = CString::new(message.as_ref()).unwrap();
        Self {
            status: OpenPublishStatusType::OpenPublishStatusTypeError,
            message: message.into_raw(),
        }
    }
}
