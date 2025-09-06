#include "openpublish/status.h"

namespace CPPOPENPUBLISH_NAMESPACE {
Status::Status() { openpublish_status_ok(this); }

Status::Status(const OpenPublishStatus &status) {
  if (status.status == OpenPublishStatusTypeOk) {
    openpublish_status_ok(this);
  } else {
    openpublish_status_error(this, status.message);
  }
}

Status::Status(const OpenPublishStatus *status) {
  if (status) {
    if (status->status == OpenPublishStatusTypeOk) {
      openpublish_status_ok(this);
    } else {
      openpublish_status_error(this, status->message);
    }
  } else {
    openpublish_status_error(this, "status is null");
  }
}

Status::~Status() { openpublish_status_destroy(this); }

bool Status::is_ok() const { return this->status == OpenPublishStatusTypeOk; }

bool Status::is_error() const {
  return this->status == OpenPublishStatusTypeError;
}

const std::string_view Status::message() const {
  return ((OpenPublishStatus *)this)->message;
}

void Status::set_ok() { openpublish_status_ok(this); }

void Status::set_error(const std::string &message) {
  openpublish_status_error(this, message.c_str());
}
void Status::set_error(const char *message) {
  openpublish_status_error(this, message);
}
} // namespace CPPOPENPUBLISH_NAMESPACE
