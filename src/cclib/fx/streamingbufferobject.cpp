#include "stdafx.h"
#include "streamingbufferobject.h"

#include <gl/glew.h>
#include <gl/glfw3.h>
#include <string>

namespace fx {

  StreamingBufferObject::StreamingBufferObject(uint32_t target, uint32_t size)
    : _target(target)
    , _size(size)
    , _vbo(0)
    , _cursor(0) {
    glGenBuffers(1, &_vbo);
    glBindBuffer(_target, _vbo);
    glBufferData(_target, _size, nullptr, GL_DYNAMIC_DRAW);
  }

  StreamingBufferObject::~StreamingBufferObject( ) {
    if (_vbo != 0) glDeleteBuffers(1, &_vbo);
  }

  const uint32_t StreamingBufferObject::Vbo( ) {
    return _vbo;
  }

  uint32_t StreamingBufferObject::StreamImpl(void* start, uint32_t elementSize, uint32_t elementCount) {
    auto bytes = elementSize * elementCount;
    auto aligned = bytes + bytes % 64;

    glBindBuffer(_target, _vbo);

    if (_cursor + aligned > _size) {
      // Orphan the current buffer and get a new one.
      glBufferData(_target, _size, nullptr, GL_DYNAMIC_DRAW);
      _cursor = 0;
    }

    auto mapped = glMapBufferRange(_target, _cursor, aligned, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
    std::memcpy(mapped, start, bytes);
    _cursor += aligned;
    glUnmapBuffer(_target);

    return _cursor - aligned;
  }
}