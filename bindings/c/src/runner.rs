use crate::{openpublish_status_ok, OpenPublishBasePublish, OpenPublishContext, OpenPublishStatus};
use std::ptr::{null, null_mut};

#[no_mangle]
pub unsafe extern "C" fn openpublish_run(
    publish: *const OpenPublishBasePublish,
    status: *mut OpenPublishStatus,
) -> *mut OpenPublishContext {
    openpublish_status_ok(status);

    let publish = match publish.as_ref() {
        Some(publish) => publish,
        None => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error("publish is null");
            }
            null()
        }
    };

    let rt = match tokio::runtime::Builder::new_current_thread().build() {
        Ok(rt) => rt,
        Err(err) => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error(format!("failed to build runtime: {}", err));
            }
            return null_mut();
        }
    };

    let result = rt.block_on(async { base_openpublish::run(unsafe { &*publish }).await });

    match result {
        Ok(context) => {
            let c_context = OpenPublishContext::from(context);
            Box::into_raw(Box::new(c_context))
        }
        Err(err) => {
            if !status.is_null() {
                *status = OpenPublishStatus::new_error(err.to_string());
            }
            null_mut()
        }
    }
}
