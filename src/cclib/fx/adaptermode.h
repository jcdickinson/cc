#pragma once
#include "adapterinfo.h"

namespace fx {

  struct AdapterMode {

    const AdapterInfo Adapter;

    const uint32_t Width, Height;
    const int32_t RefreshRate;

    const uint8_t RedBits, GreenBits, BlueBits;

    inline AdapterMode(const uint32_t width = 640, const uint32_t height = 480,
                       const int8_t redBits = 8, const int8_t greenBits = 8, const int8_t blueBits = 8,
                       const int32_t refreshRate = -1, const AdapterInfo& adapter = AdapterInfo( ))
                       : Adapter(adapter)
                       , Width(width)
                       , Height(height)
                       , RefreshRate(refreshRate)
                       , RedBits(redBits)
                       , GreenBits(greenBits)
                       , BlueBits(blueBits) {
    }

    inline ~AdapterMode( ) {

    }

  };

}