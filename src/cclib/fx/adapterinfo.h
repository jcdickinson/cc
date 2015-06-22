#pragma once
#include <string>
#include <stdint.h>

namespace fx {

  struct AdapterInfo {
    const void* Native;
    const std::string Title;
    const bool Fullscreen;
    const bool IsDefault;

    AdapterInfo(const AdapterInfo&) = default;
    AdapterInfo& operator=(const AdapterInfo&) = delete;

    AdapterInfo(void* native = nullptr, std::string title = "", bool fullscreen = false, bool isDefault = false)
      : Native(native)
      , Title(title)
      , Fullscreen(fullscreen)
      , IsDefault(isDefault) {

    }

    inline ~AdapterInfo( ) {

    }
  };

}