#pragma once
#include <exception>
#include <stdint.h>
#include <string>

enum class ErrorCode {
  UNKNOWN = -1,

  // -------------------
  // -- FX EXCEPTIONS --
  // -------------------
  FX_LOW = 0x0,
  FX_HIGH = 0xFE,

  FX_SHADER_COMPILE_FAILURE = FX_LOW + 0x1,

  CONTENT_LOW = 0xFF,
  CONTENT_HIGH = 0x1FD,

  CONTENT_NOT_FOUND = CONTENT_LOW + 0x1,
  CONTENT_INVALID_PATH = CONTENT_LOW + 0x2,
  CONTENT_INVALID_DATA = CONTENT_LOW + 0x3
};

class EngineException : public std::exception {
  public:
  EngineException(const char* message, ErrorCode code = ErrorCode::UNKNOWN);
  EngineException(const std::string& message, ErrorCode code = ErrorCode::UNKNOWN);
  ~EngineException( ) throw();
  const char* what( ) const throw() override;
  const ErrorCode code( ) const throw();

  private:
  const ErrorCode _code;
  const char* _what;
};