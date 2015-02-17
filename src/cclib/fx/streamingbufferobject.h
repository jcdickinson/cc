#pragma once
#include <stdint.h>

namespace fx {
  class StreamingBufferObject {
    public:
    StreamingBufferObject(uint32_t target, uint32_t size = 0x200000);
    ~StreamingBufferObject( );

    const uint32_t Vbo( );

    template <typename T>
    uint32_t Stream(T& first, uint32_t elementCount) {
      return StreamImpl(&first, sizeof(T), elementCount);
    }

    private:
    uint32_t StreamImpl(void* start, uint32_t elementSize, uint32_t elementCount);

    const uint32_t _target, _size;
    uint32_t _vbo, _cursor;
  };
}