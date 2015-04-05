#pragma once
#include <codecvt>
#include <iostream>
#include <locale>
#include <string>
#include <wchar.h>

#include "tools.h"

#define LOG(level) std::wcout << std::endl << #level << L" "

cclib_static_inline std::wostream &operator <<(std::wostream& left, const std::string str) {
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  auto wide = converter.from_bytes(str);
  left << wide;
  return left;
}