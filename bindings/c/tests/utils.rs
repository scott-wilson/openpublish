use std::ffi::CStr;

pub unsafe fn validate_is_ok(status: &openpublish::openpublishStatus) {
    assert_eq!(
        status.status,
        openpublish::OpenPublishStatusType::OpenPublishStatusTypeOk,
        "Err: {}",
        CStr::from_ptr(status.message).to_string_lossy()
    );
}
