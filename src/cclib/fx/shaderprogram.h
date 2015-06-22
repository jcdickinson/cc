#pragma once
#include <stdint.h>

namespace fx {

  class IShaderProgram {
    public:
    virtual const uint32_t Id( ) = 0;
  };

  template<uint32_t T> class ShaderProgram : public IShaderProgram {
    public:
    ShaderProgram(const ShaderProgram&) = default;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    ShaderProgram(const uint32_t id) :
      _id(id) {
    }

    ~ShaderProgram( ) {

    }

    const uint32_t Id( ) {
      return _id;
    }

    enum {
      ShaderType = T
    };
    private:
    const uint32_t _id;
  };

  typedef ShaderProgram<0x8B30> FragmentShaderProgram;
  typedef ShaderProgram<0x8B31> VertexShaderProgram;

}