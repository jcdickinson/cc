#pragma once
#include "../math.h"

namespace fx {
  class Camera {

    public:
    Camera(const math::vec2& viewport = { 1.0f, 1.0f }, const math::vec2& extents = { 0.0f, 0.0f });
    ~Camera( );

    math::mat4 Matrix( );
    math::vec2& Position( );
    math::vec2& Viewport( );
    math::vec2& Extents( );

    private:
    math::vec2 _position;
    math::vec2 _viewport;
    math::vec2 _extents;
  };
}