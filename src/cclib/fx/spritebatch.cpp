#include "stdafx.h"
#include "spritebatch.h"

#include <gl/glew.h>
#include <gl/glfw3.h>

namespace fx {

# define BUFFER_OFFSET(i) ((char*)nullptr + (i))

  SpriteBatch::SpriteBatch( )
    : _vao(0)
    , _vertices(GL_ARRAY_BUFFER)
    , _indices(GL_ELEMENT_ARRAY_BUFFER) {

    glGenVertexArrays(1, &_vao);
  }

  SpriteBatch::~SpriteBatch( ) {
    if (_vao != 0) glDeleteVertexArrays(1, &_vao);
  }

  void SpriteBatch::Begin( ) {
    _verticesSource.clear( );
    _indicesSource.clear( );
  }

  void SpriteBatch::End( ) {
    Flush(true);
  }

  void SpriteBatch::Draw(float x, float y, float z, float w, float h) {
    index_t i1 = _verticesSource.size( ),
            i2 = i1 + 1, i3 = i2 + 1, i4 = i3 + 1;

    _verticesSource.push_back({ x + 0, y + 0, z, 0, 0 });
    _verticesSource.push_back({ x + w, y + 0, z, 0, 0 });
    _verticesSource.push_back({ x + w, y + h, z, 0, 0 });
    _verticesSource.push_back({ x + 0, y + h, z, 0, 0 });

    _indicesSource.push_back(i1);
    _indicesSource.push_back(i3);
    _indicesSource.push_back(i4);

    _indicesSource.push_back(i1);
    _indicesSource.push_back(i2);
    _indicesSource.push_back(i3);

    if (_verticesSource.size( ) >= 1024) {
      Flush(true);
    }
  }

  void SpriteBatch::Flush(bool forced) {
    if (_verticesSource.size( ) != 0) {
      glBindVertexArray(_vao);

      auto voffset = _vertices.Stream<SpriteVertex>(_verticesSource[0], _verticesSource.size( ));
      auto ioffset = _indices.Stream<index_t>(_indicesSource[0], _indicesSource.size( ));

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), BUFFER_OFFSET(voffset));
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), BUFFER_OFFSET(voffset + 12));
      glEnableVertexArrayAttrib(_vao, 0);
      glEnableVertexArrayAttrib(_vao, 1);

      glDrawElements(GL_TRIANGLES, _indicesSource.size( ), GL_UNSIGNED_INT, BUFFER_OFFSET(ioffset));

      glDisableVertexArrayAttrib(_vao, 0);
      glDisableVertexArrayAttrib(_vao, 1);

      _verticesSource.clear( );
      _indicesSource.clear( );
    }
  }

}