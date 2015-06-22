#include "stdafx.h"
#include "shaderprogram.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include <gl/glew.h>
#include <gl/glfw3.h>

#include "../tools.h"
#include "../logging.h"
#include "../content/contentmanager.h"

using namespace std;

namespace fx {

}

namespace content {

  template<uint32_t T> shared_ptr<fx::ShaderProgram<T>> LoadShader(const LoadOperation& operation) {
    uint32_t shaderId = glCreateShader(T);

    LOG(DEBUG) << L"Reading shader " << operation.Path << L" ...";
    string shaderCode;
    operation.Data->seekg(0, std::ios::end);
    shaderCode.reserve(operation.Data->tellg( ));
    operation.Data->seekg(0, std::ios::beg);
    shaderCode.assign((std::istreambuf_iterator<char>(*operation.Data.get( ))), std::istreambuf_iterator<char>( ));
    
    LOG(DEBUG) << L"Compiling shader " << operation.Path << L" ...";
    char const * shaderSourcePointer = shaderCode.c_str( );
    glShaderSource(shaderId, 1, &shaderSourcePointer, nullptr);
    glCompileShader(shaderId);

    GLint result = GL_FALSE;
    int infoLogLength;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);

    if (infoLogLength > 0) {
      auto errorMessage = vector<char>(infoLogLength + 1);
      glGetShaderInfoLog(shaderId, infoLogLength, NULL, &errorMessage[0]);
      if (errorMessage[0] != '\0')
        LOG(ERROR) << &errorMessage[0];
    }

    if (!result) {
      glDeleteShader(shaderId);
      throw EngineException("Failed to compile shader: " + operation.Path, ErrorCode::FX_SHADER_COMPILE_FAILURE);
    }

    return make_shared<fx::ShaderProgram<T>>(shaderId);
  }

  template<> shared_ptr<fx::FragmentShaderProgram> ContentManager::LoadContent<fx::FragmentShaderProgram>(const LoadOperation& operation) {
    return LoadShader<fx::FragmentShaderProgram::ShaderType>(operation);
  }

  template<> shared_ptr<fx::VertexShaderProgram> ContentManager::LoadContent<fx::VertexShaderProgram>(const LoadOperation& operation) {
    return LoadShader<fx::VertexShaderProgram::ShaderType>(operation);
  }

  template<> const string ContentManager::ContentExtension<fx::FragmentShaderProgram>( ) {
    return ".glsl";
  }

  template<> const string ContentManager::ContentExtension<fx::VertexShaderProgram>( ) {
    return ".glsl";
  }

}