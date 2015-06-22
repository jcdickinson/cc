#pragma once
#include <stdint.h>

namespace fx
{
  class Texture {
    Texture(const Texture&) = default;
    Texture& operator=(const Texture&) = delete;

    public:
    Texture(const uint32_t id, const uint32_t width, const uint32_t height, const uint32_t linearSize, const uint32_t mipmapCount);
    ~Texture( );

    const uint32_t Id( );
    const uint32_t Width( );
    const uint32_t Height( );
    const uint32_t LinearSize( );
    const uint32_t MipMapCount( );

    private:
    const uint32_t _id;
    const uint32_t _width;
    const uint32_t _height;
    const uint32_t _linearSize;
    const uint32_t _mipMapCount;
  };
}