#include "stdafx.h"
#include "camera.h"

namespace fx {

  using namespace math;

  Camera::Camera(const vec2& viewport, const vec2& extents)
    : _position(vec2(0.0f, 0.0f))
    , _viewport(viewport)
    , _extents(extents) {

  }

  Camera::~Camera( ) {

  }

  mat4 Camera::Matrix( ) {
    auto pos = _position;

    if (_extents.x( ) > 0 && _extents.y( ) > 0) {
      const auto half = _viewport * 0.5f;
      pos = pos - half;
      const auto max = _extents - _viewport;
      if (pos.x( ) < 0) pos.x( ) = 0;
      if (pos.y( ) < 0) pos.y( ) = 0;
      if (pos.x( ) > max.x( )) pos.x( ) = max.x( );
      if (pos.y( ) > max.y( )) pos.y( ) = max.y( );
      pos = pos + half;
    }

    const auto ortho = mat_ortho(0.0f, _viewport.x( ), 0.0f, _viewport.y( ));
    const auto trans = mat_translate<4>(vec3(-pos.x( ), -pos.y( ), 0));
    return ortho * trans;
  }

}