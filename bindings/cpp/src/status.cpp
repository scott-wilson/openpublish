#include "cpppublish/status.h"

namespace CPPPUBLISH_NAMESPACE {
Status::Status() { cpublish_status_ok(this); }

Status::Status(const CPublishStatus &status) {
  if (status.status == CPublishStatusTypeOk) {
    cpublish_status_ok(this);
  } else {
    cpublish_status_error(this, status.message);
  }
}

Status::Status(const CPublishStatus *status) {
  if (status) {
    if (status->status == CPublishStatusTypeOk) {
      cpublish_status_ok(this);
    } else {
      cpublish_status_error(this, status->message);
    }
  } else {
    cpublish_status_error(this, "status is null");
  }
}

Status::~Status() { cpublish_status_destroy(this); }

bool Status::is_ok() const { return this->status == CPublishStatusTypeOk; }

bool Status::is_error() const {
  return this->status == CPublishStatusTypeError;
}

const std::string_view Status::message() const {
  return ((CPublishStatus *)this)->message;
}

void Status::set_ok() { cpublish_status_ok(this); }

void Status::set_error(const std::string &message) {
  cpublish_status_error(this, message.c_str());
}
void Status::set_error(const char *message) {
  cpublish_status_error(this, message);
}
} // namespace CPPPUBLISH_NAMESPACE
