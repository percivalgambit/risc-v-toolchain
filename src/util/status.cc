#include "util/status.h"

// Copied from https://github.com/google/lmctfy/blob/master/util/task/status.cc

#include <ostream>
#include <string>

namespace util {

const Status& Status::OK = Status();

Status::Status() : Status(Code::kOK, "") {}

Status::Status(const Status::Code code) : Status(code, "") {}

Status::Status(const Status::Code code, const std::string& message)
    : code_(code), message_(message) {}

bool Status::operator==(const Status& other) const {
  return (this->code_ == other.code_) && (this->message_ == other.message_);
}

bool Status::operator!=(const Status& other) const { return !(*this == other); }

bool Status::IsOk() const { return code_ == Status::Code::kOK; }

Status::Code Status::StatusCode() const { return code_; }

const std::string& Status::Message() const { return message_; }

std::string Status::ToString() const {
  if (code_ == Status::Code::kOK) {
    return "OK";
  }

  return std::string(CodeStrings[static_cast<int>(code_)]) + ": " + message_;
}

std::ostream& operator<<(std::ostream& os, const Status::Code code) {
  os << Status::CodeStrings[static_cast<int>(code)];
  return os;
}

std::ostream& operator<<(std::ostream& os, const Status& status) {
  os << status.ToString();
  return os;
}

}  // namespace util
