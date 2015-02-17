#include "stdafx.h"
#include "engineexception.h"

const char* copy(const std::string& str) {
  auto l = str.length( ) + 1;
  auto cstr = new char[l];
  strcpy_s(cstr, l, str.c_str( ));
  return cstr;
}

const char* copy(const char* message) {
  std::string s = message;
  return copy(s);
}

EngineException::EngineException(const std::string& message, int32_t code)
  : _what(copy(message))
  , _code(code) {

}

EngineException::EngineException(const char* message, int32_t code)
  : _what(copy(message))
  , _code(code) {

}

EngineException::~EngineException( ) {
  
}

const char* EngineException::what( ) const throw() {
  return _what;
}

const int32_t EngineException::code( ) const throw() {
  return _code;
}