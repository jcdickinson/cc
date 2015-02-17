#pragma once
#include <exception>
#include <stdint.h>
#include <string>

class EngineException : public std::exception {
  public:
  EngineException(const char* message, int32_t code = 0);
  EngineException(const std::string& message, int32_t code = 0);
  ~EngineException( ) throw();
  const char* what( ) const throw() override;
  const int32_t code( ) const throw();

  private:
  const int32_t _code;
  const char* _what;
};