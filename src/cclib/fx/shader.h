#pragma once
#include <memory>
#include <stdint.h>
#include <string>
#include <unordered_map>

#include "shaderprogram.h"

namespace fx {

  class Shader {
    public:
    Shader(const uint32_t id, const std::unordered_map<uint32_t, std::shared_ptr<IShaderProgram>>& programs);
    ~Shader();

    uint32_t Uniform(const std::string name);
    template<typename T> void Uniform(const uint32_t id, const T& value);
    template<typename T> void Uniform(const std::string name, const T& value);

    const uint32_t Id( );
    template<typename T> std::shared_ptr<T> Program( );

    void Apply( );

    private:
    const uint32_t _id;
    const std::unordered_map<uint32_t, std::shared_ptr<IShaderProgram>> _programs;
  };

  template<typename T> std::shared_ptr<T> Shader::Program( ) {
    auto val = _programs.find(T::ShaderType);
    if (val == _programs.end( )) return nullptr;
    return std::static_pointer_cast<T>(val->second);
  }

  template<typename T> void Shader::Uniform(const std::string name, const T& value) {
    Uniform(Uniform(name), value);
  }

}
