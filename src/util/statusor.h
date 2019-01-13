#ifndef UTIL_STATUSOR_H_
#define UTIL_STATUSOR_H_

// Copied from https://github.com/google/lmctfy/blob/master/util/task/statusor.h

#include <variant>

#include "util/logging.h"
#include "util/status.h"

namespace util {

template <typename T>
class [[nodiscard]] StatusOr {
 public:
  StatusOr(const Status& status);

  StatusOr(const T& value);

  template <typename U>
  StatusOr(const StatusOr<U>& other);

  template <typename U>
  const StatusOr& operator=(const StatusOr<U>& other);

  const Status& GetStatus() const;

  bool IsOk() const;

  const T& ValueOrDie() const;

  template <typename U>
  friend class StatusOr;

 private:
  std::variant<T, Status> value_;
};

template <typename T>
StatusOr<T>::StatusOr(const Status& status) : value_(status) {
  if (status.IsOk()) {
    FATAL("Status::OK is not a valid argument to StatusOr");
  }
}

template <typename T>
StatusOr<T>::StatusOr(const T& value) : value_(value) {}

template <typename T>
template <typename U>
StatusOr<T>::StatusOr(const StatusOr<U>& other) {
  value_ = std::visit([](auto&& arg) -> std::variant<T, Status> { return arg; },
                      other.value_);
}

template <typename T>
template <typename U>
const StatusOr<T>& StatusOr<T>::operator=(const StatusOr<U>& other) {
  value_ = std::visit([](auto&& arg) -> std::variant<T, Status> { return arg; },
                      other.value_);
  return *this;
}

template <typename T>
const Status& StatusOr<T>::GetStatus() const {
  if (std::holds_alternative<T>(value_)) {
    return Status::OK;
  }
  return std::get<Status>(value_);
}

template <typename T>
bool StatusOr<T>::IsOk() const {
  return std::holds_alternative<T>(value_);
}

template <typename T>
const T& StatusOr<T>::ValueOrDie() const {
  if (!std::holds_alternative<T>(value_)) {
    FATAL("Attempting to fetch value of non-OK StatusOr");
  }
  return std::get<T>(value_);
}

}  // namespace util

#endif  // UTIL_STATUSOR_H_
