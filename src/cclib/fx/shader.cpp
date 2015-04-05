#include "stdafx.h"
#include "shader.h"

#include <cassert>
#include <stdint.h>
#include <unordered_set>

#include <gl/glew.h>
#include <gl/glfw3.h>
#include <rapidjson/document.h>

#include "../logging.h"
#include "../content/contentmanager.h"
#include "../math.h"

using namespace std;

namespace fx {

  Shader::Shader(const uint32_t id, const unordered_map<uint32_t, std::shared_ptr<IShaderProgram>>& programs) :
    _id(id),
    _programs(programs) {

  }

  Shader::~Shader( ) {

  }

  const uint32_t Shader::Id( ) {
    return _id;
  }

  void Shader::Apply( ) {
    glUseProgram(_id);
  }

  uint32_t Shader::Uniform(const std::string name) {
    return glGetUniformLocation(_id, name.c_str( ));
  }

  template<> void Shader::Uniform<math::mat2>(const uint32_t id, const math::mat2& value) {
    glProgramUniformMatrix2fv(_id, id, 1, GL_FALSE, &value[0]);
  }

  template<> void Shader::Uniform<math::mat3>(const uint32_t id, const math::mat3& value) {
    glProgramUniformMatrix3fv(_id, id, 1, GL_FALSE, &value[0]);
  }

  template<> void Shader::Uniform<math::mat4>(const uint32_t id, const math::mat4& value) {
    glProgramUniformMatrix4fv(_id, id, 1, GL_FALSE, &value[0]);
  }
}

namespace content {

  class IStreamWrapper {
    public:
    typedef char Ch;

    IStreamWrapper(istream& is) : _is(is) {
    }

    Ch Peek( ) const {
      int c = _is.peek( );
      return c == char_traits<char>::eof( ) ? '\0' : (Ch) c;
    }
    Ch Take( ) {
      int c = _is.get( );
      return c == char_traits<char>::eof( ) ? '\0' : (Ch) c;
    }
    size_t Tell( ) const {
      return (size_t) _is.tellg( );
    }

    Ch* PutBegin( ) {
      assert(false); return 0;
    }
    void Put(Ch) {
      assert(false);
    }
    void Flush( ) {
      assert(false);
    }
    size_t PutEnd(Ch*) {
      assert(false); return 0;
    }

    private:
    IStreamWrapper(const IStreamWrapper&);
    IStreamWrapper& operator=(const IStreamWrapper&);
    istream& _is;
  };

  template<> shared_ptr<fx::Shader> ContentManager::LoadContent<fx::Shader>(const LoadOperation& operation) {
    IStreamWrapper stream(*operation.Data.get( ));
    rapidjson::Document d;
    d.ParseStream(stream);

    auto err = d.HasParseError( );

    auto sources = d.FindMember("sources");
    if (sources == d.MemberEnd( ) || !sources->value.IsObject( ))
      throw EngineException("Expected 'sources' object member: " + operation.Path, ErrorCode::CONTENT_INVALID_DATA);

    unordered_set<string> seenMembers;
    unordered_map<uint32_t, shared_ptr<fx::IShaderProgram>> programs;
    for (auto it = sources->value.MemberBegin( ); it != sources->value.MemberEnd( ); ++it) {
      auto name = string(it->name.GetString( ));
      auto& value = it->value;

      if (!seenMembers.insert(name).second)
        throw EngineException("The 'sources." + name + "' member is duplicated: " + operation.Path, ErrorCode::CONTENT_INVALID_DATA);
      if (!value.IsString( ))
        throw EngineException("The 'sources." + name + "' member must be a string: " + operation.Path, ErrorCode::CONTENT_INVALID_DATA);

      if (name == "fragment") {
        programs[fx::FragmentShaderProgram::ShaderType] = 
          operation.ContentManager.LoadContent<fx::FragmentShaderProgram>(operation.Path, string(value.GetString( )));
      } else if (name == "vertex") {
        programs[fx::VertexShaderProgram::ShaderType] =
          operation.ContentManager.LoadContent<fx::VertexShaderProgram>(operation.Path, string(value.GetString( )));
      } else {
        throw EngineException("The 'sources." + name + "' is not supported: " + operation.Path, ErrorCode::CONTENT_INVALID_DATA);
      }
    }

    GLint result = GL_FALSE;
    int infoLogLength;

    auto programId = glCreateProgram( );
    for (auto it = programs.begin( ); it != programs.end( ); ++it) {
      glAttachShader(programId, it->second->Id( ));
    }
    glLinkProgram(programId);

    glGetProgramiv(programId, GL_LINK_STATUS, &result);
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);

    if (infoLogLength > 0) {
      auto errorMessage = vector<char>(infoLogLength + 1);
      glGetProgramInfoLog(programId, infoLogLength, NULL, &errorMessage[0]);
      if (errorMessage[0] != '\0')
        LOG(ERROR) << &errorMessage[0];
    }

    if (!result) {
      glDeleteProgram(programId);
      throw EngineException("Failed to link shader program: " + operation.Path, ErrorCode::FX_SHADER_COMPILE_FAILURE);
    }

    return make_shared<fx::Shader>(programId, programs);
  }

  template<> const string ContentManager::ContentExtension<fx::Shader>( ) {
    return ".json";
  }

}