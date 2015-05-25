#include "stdafx.h"
#include "shader.h"

#include <cassert>
#include <stdint.h>
#include <string>
#include <unordered_set>

#include <gl/glew.h>
#include <gl/glfw3.h>
#include <rapidjson/document.h>

#include "../logging.h"
#include "../content/contentmanager.h"
#include "../math.h"

using namespace std;

namespace fx {

  class FragmentShaderState : public IGpuState {
    private:
    const bool _blend_enabled;
    const uint32_t _blend_src;
    const uint32_t _blend_dst;

    public:
    FragmentShaderState(bool blend_enabled = false, uint32_t blend_src = 0, uint32_t blend_dst = 0) :
      _blend_enabled(blend_enabled),
      _blend_src(blend_src),
      _blend_dst(blend_dst) {

    }

    void Apply( ) override {
      if (_blend_enabled) {
        glEnable(GL_BLEND);
        glBlendFunc(_blend_src, _blend_dst);
      } else {
        glDisable(GL_BLEND);
      }
    }
  };

  Shader::Shader(
    const uint32_t id,
    const unordered_map<uint32_t, shared_ptr<IShaderProgram>>& programs,
    const vector<shared_ptr<IGpuState>>& states) :
    _id(id),
    _programs(programs),
    _states(states) {

  }

  Shader::~Shader( ) {

  }

  const uint32_t Shader::Id( ) {
    return _id;
  }

  void Shader::Apply( ) {
    glUseProgram(_id);
    for (auto it = _states.begin( ); it != _states.end( ); ++it) {
      (*it)->Apply( );
    }
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

  uint32_t ReadBlendFunc(const LoadOperation& operation, rapidjson::Value& value, bool& enabled) {
    auto str = string(value.GetString( ));
    enabled = true;
    if (str == "one") return GL_ONE;
    if (str == "zero") return GL_ZERO;
    if (str == "src_color") return GL_SRC_COLOR;
    if (str == "src_alpha") return GL_SRC_ALPHA;
    if (str == "dst_color") return GL_DST_COLOR;
    if (str == "dst_alpha") return GL_DST_ALPHA;
    if (str == "one_minus_src_color") return GL_ONE_MINUS_SRC_COLOR;
    if (str == "one_minus_src_alpha") return GL_ONE_MINUS_SRC_ALPHA;
    if (str == "one_minus_dst_color") return GL_ONE_MINUS_DST_COLOR;
    if (str == "one_minus_dst_alpha") return GL_ONE_MINUS_DST_ALPHA;
    enabled = false;
    return 0;
  }

  shared_ptr<fx::IGpuState> ReadFragmentState(const LoadOperation& operation, rapidjson::Value& value) {
    bool blend_enabled = 0;
    uint32_t blend_src = GL_ONE;
    uint32_t blend_dst = GL_ZERO;

    if (value.HasMember("blend")) {
      auto& blend = value["blend"];
      if (!blend.IsObject( ))
        throw EngineException("The 'sources.fragment.blend' member must be a object if present: " + operation.Path, ErrorCode::CONTENT_INVALID_DATA);

      if (blend.HasMember("src")) {
        auto& jsrc = blend["src"];
        if (!jsrc.IsString( ))
          throw EngineException("The 'sources.fragment.blend.src' member must be a string if present: " + operation.Path, ErrorCode::CONTENT_INVALID_DATA);
        blend_src = ReadBlendFunc(operation, jsrc, blend_enabled);
        if (!blend_enabled)
          throw EngineException("The 'sources.fragment.blend.src' is not a valid value: " + operation.Path, ErrorCode::CONTENT_INVALID_DATA);
      }

      if (blend.HasMember("dst")) {
        auto& jdst = blend["dst"];
        if (!jdst.IsString( ))
          throw EngineException("The 'sources.fragment.blend.dst' member must be a string if present: " + operation.Path, ErrorCode::CONTENT_INVALID_DATA);
        blend_dst = ReadBlendFunc(operation, jdst, blend_enabled);
        if (!blend_enabled)
          throw EngineException("The 'sources.fragment.blend.dst' is not a valid value: " + operation.Path, ErrorCode::CONTENT_INVALID_DATA);
      }
    }

    return make_shared<fx::FragmentShaderState>(blend_enabled, blend_src, blend_dst);
  }

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
    vector<shared_ptr<fx::IGpuState>> states;

    for (auto it = sources->value.MemberBegin( ); it != sources->value.MemberEnd( ); ++it) {
      auto name = string(it->name.GetString( ));
      auto& value = it->value;

      if (!seenMembers.insert(name).second)
        throw EngineException("The 'sources." + name + "' member is duplicated: " + operation.Path, ErrorCode::CONTENT_INVALID_DATA);
      if (!value.IsObject( ))
        throw EngineException("The 'sources." + name + "' member must be an object: " + operation.Path, ErrorCode::CONTENT_INVALID_DATA);
      if (!value.HasMember("file"))
        throw EngineException("The 'sources." + name + "' member must have a file member: " + operation.Path, ErrorCode::CONTENT_INVALID_DATA);
      auto& file = value["file"];
      if (!file.IsString( ))
        throw EngineException("The 'sources." + name + "' member must have a file member that is a string: " + operation.Path, ErrorCode::CONTENT_INVALID_DATA);

      if (name == "fragment") {

        programs[fx::FragmentShaderProgram::ShaderType] =
          operation.ContentManager.LoadContent<fx::FragmentShaderProgram>(operation.Path, string(file.GetString( )));
        states.push_back(ReadFragmentState(operation, value));

      } else if (name == "vertex") {

        programs[fx::VertexShaderProgram::ShaderType] =
          operation.ContentManager.LoadContent<fx::VertexShaderProgram>(operation.Path, string(file.GetString( )));

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

    return make_shared<fx::Shader>(programId, programs, states);
  }

  template<> const string ContentManager::ContentExtension<fx::Shader>( ) {
    return ".json";
  }

}