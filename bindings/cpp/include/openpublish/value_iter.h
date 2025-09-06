#pragma once

extern "C" {
#include <copenpublish.h>
}

#include <iterator>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "openpublish/status.h"
#include "openpublish/value.h"

namespace CPPOPENPUBLISH_NAMESPACE {
class Value;
class ValueView;

class ValueIterArray {
public:
  ValueIterArray(const CPPOPENPUBLISH_NAMESPACE::Value &value,
                 CPPOPENPUBLISH_NAMESPACE::Status &status);

  ValueIterArray(const CPPOPENPUBLISH_NAMESPACE::ValueView &value,
                 CPPOPENPUBLISH_NAMESPACE::Status &status);

  ~ValueIterArray();

  bool is_done(CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  void next(CPPOPENPUBLISH_NAMESPACE::Status &status);

  const ValueView value(CPPOPENPUBLISH_NAMESPACE::Status &status) const;

private:
  OpenPublishValueIterArray *_iter;
};

class ValueIterObject {
public:
  ValueIterObject(const CPPOPENPUBLISH_NAMESPACE::Value &value,
                  CPPOPENPUBLISH_NAMESPACE::Status &status);

  ValueIterObject(const CPPOPENPUBLISH_NAMESPACE::ValueView &value,
                  CPPOPENPUBLISH_NAMESPACE::Status &status);

  ~ValueIterObject();

  bool is_done(CPPOPENPUBLISH_NAMESPACE::Status &status) const;

  void next(CPPOPENPUBLISH_NAMESPACE::Status &status);

  const std::string_view key(CPPOPENPUBLISH_NAMESPACE::Status &status) const;
  const ValueView value(CPPOPENPUBLISH_NAMESPACE::Status &status) const;

private:
  OpenPublishValueIterObject *_iter;
};
} // namespace CPPOPENPUBLISH_NAMESPACE
