//
// Created by robcholz on 3/31/23.
//

#ifndef MINECRAFT_ERRORHANDLING_HPP
#define MINECRAFT_ERRORHANDLING_HPP

#include <exception>
#include <string>
#include <utility>

enum ExceptionType : unsigned char {
  EXCEPTION_NULLPTR,
  EXCEPTION_ERROR_TYPE,
  EXCEPTION_INVALID_ARGUMENT,
  EXCEPTION_FAILED_TO_LOCATE_FILE,
  EXCEPTION_DIRECTORY_DOES_NOT_EXIST
};

namespace error {
class Exception : public std::exception {
 public:
  [[nodiscard]] const char* what() const noexcept override { return m.what(); }

  const int id;

 protected:
  explicit Exception(int id, const char* whatArg) : id(id), m(whatArg) {}

  static std::string name(const std::string& eName, int id) {
    return "[minecraft.Exception." + eName + "." + std::to_string(id) + "] ";
  }

 private:
  std::runtime_error m;
};

class TypeErrorException : public Exception {
 public:
  static TypeErrorException create(const std::string& whatArg) {
    std::string w =
        Exception::name("type_error", ExceptionType::EXCEPTION_ERROR_TYPE) +
        whatArg;
    return {TypeErrorException(w.c_str())};
  }

 private:
  explicit TypeErrorException(const char* whatArg)
      : Exception(ExceptionType::EXCEPTION_ERROR_TYPE, whatArg) {}
};

class NullPtrException : public Exception {
 public:
  static NullPtrException create(const std::string& whatArg) {
    std::string w =
        Exception::name("NullPtrException", ExceptionType::EXCEPTION_NULLPTR) +
        whatArg;
    return {NullPtrException(w.c_str())};
  }

 private:
  explicit NullPtrException(const char* whatArg)
      : Exception(ExceptionType::EXCEPTION_NULLPTR, whatArg) {}
};

class ChunkDoesntExistException : public std::exception {
 public:
  ChunkDoesntExistException(std::string message)
      : message_(std::move(message)) {}

  [[nodiscard]] const char* what() const noexcept override {
    return message_.c_str();
  }

 private:
  std::string message_;
};
}  // namespace error

#endif  // MINECRAFT_ERRORHANDLING_HPP
