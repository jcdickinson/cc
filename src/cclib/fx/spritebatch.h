#pragma once
#include <stdint.h>
#include <vector>

#include "streamingbufferobject.h"
#include "../math.h"

namespace fx {

  struct SpriteVertex {

    float x, y, z;
    float u, v;

    uint8_t reserved[12];

  };

  class SpriteBatch {
    public:
    typedef uint32_t index_t;

    SpriteBatch(const SpriteBatch&) = default;
    SpriteBatch& operator=(const SpriteBatch&) = delete;

    SpriteBatch( );
    ~SpriteBatch( );

    void Begin(math::mat4 matrix);
    void End( );

    void Draw(float x, float y, float z, float w, float h);

    private:
    math::mat4 _matrix;

    uint32_t _vao;
    StreamingBufferObject _vertices;
    StreamingBufferObject _indices;

    std::vector<SpriteVertex> _verticesSource;
    std::vector<index_t> _indicesSource;

    void Flush();
  };

}