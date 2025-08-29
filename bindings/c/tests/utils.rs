use std::ffi::CStr;

pub unsafe fn validate_is_ok(status: &cpublish::CPublishStatus) {
    assert_eq!(
        status.status,
        cpublish::CPublishStatusType::CPublishStatusTypeOk,
        "Err: {}",
        CStr::from_ptr(status.message).to_string_lossy()
    );
}
