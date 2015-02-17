#pragma once
#include <stdint.h>
#include "streamingbufferobject.h"
#include <vector>

namespace fx {

  struct SpriteVertex {

    float x, y, z;
    float u, v;

    uint8_t reserved[12];

  };

  class SpriteBatch {
    public:
    typedef uint32_t index_t;

    SpriteBatch( );
    ~SpriteBatch( );

    void Begin( );
    void End( );

    void Draw(float x, float y, float z, float w, float h);

    private:
    uint32_t _vao;
    StreamingBufferObject _vertices;
    StreamingBufferObject _indices;
    
    std::vector<SpriteVertex> _verticesSource;
    std::vector<index_t> _indicesSource;

    void Flush(bool forced);
  };

}