#include "stdafx.h"
#include "context.h"

#include <gl/glew.h>
#include <gl/glfw3.h>

#include <string>

#include "../logging.h"

#define WND reinterpret_cast<GLFWwindow*>(_native)

using namespace std;

namespace fx {

  static int _glfwInit;
  static int _glewInit;
  static int _lastErrorCode;
  static string _lastErrorString;

  void ErrorCallback(int errorCode, const char* message) {
    _lastErrorCode = errorCode;
    _lastErrorString = message;
  }

  Context::Context(const ContextOptions& options, string title)
  : _options(options)
  , _native(nullptr) {

    if ((++_glfwInit) == 1) {
      glfwSetErrorCallback(&ErrorCallback);
      if (!glfwInit( )) {
        _glfwInit--;
        throw EngineException("Failed to prepare the OpenGL context: " + _lastErrorString, (ErrorCode)_lastErrorCode);
      }
    }

    glfwWindowHint(GLFW_RED_BITS, _options.Mode.RedBits);
    glfwWindowHint(GLFW_GREEN_BITS, _options.Mode.GreenBits);
    glfwWindowHint(GLFW_BLUE_BITS, _options.Mode.BlueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, _options.Mode.RefreshRate);

    glfwWindowHint(GLFW_ALPHA_BITS, _options.AlphaBits);
    glfwWindowHint(GLFW_STENCIL_BITS, _options.StencilBits);
    glfwWindowHint(GLFW_DEPTH_BITS, _options.DepthBits);
    glfwWindowHint(GLFW_SAMPLES, _options.Samples);
    glfwWindowHint(GLFW_DOUBLEBUFFER, _options.DoubleBuffer ? GL_TRUE : GL_FALSE);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, _options.Debug ? GL_TRUE : GL_FALSE);

    glfwWindowHint(GLFW_RESIZABLE, _options.Resizeable ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_VISIBLE, _options.Visible ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_DECORATED, _options.Decorated ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_FOCUSED, _options.Focused ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_AUTO_ICONIFY, _options.AutoIconify ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_FLOATING, _options.Topmost ? GL_TRUE : GL_FALSE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _native = glfwCreateWindow(_options.Mode.Width, _options.Mode.Height, title.c_str( ),
                               reinterpret_cast<GLFWmonitor*>(const_cast<void*>(_options.Mode.Adapter.Native)),
                               nullptr);
    if (!_native) {
      throw EngineException("Failed to create the OpenGL context: " + _lastErrorString, (ErrorCode)_lastErrorCode);
    }

    glfwMakeContextCurrent(WND);

    if ((++_glewInit) == 1) {
      glewExperimental = true;
      auto rc = glewInit( );
      if (rc != GLEW_OK) {
        throw EngineException("Failed to initialize OpenGL extensions.", (ErrorCode) rc);
      }
    }

    glfwSetInputMode(WND, GLFW_STICKY_KEYS, GL_TRUE);
  }

  Context::~Context( ) {
    if (_native) {
      glfwDestroyWindow(WND);
      _native = nullptr;
    }
    if ((--_glfwInit) == 0) {
      glfwTerminate( );
    }
    if ((--_glewInit) == 0) {

    }
  }

  void Context::Begin( ) {
    glfwMakeContextCurrent(WND);
  }

  void Context::End( ) {
    glfwSwapBuffers(WND);
    glfwPollEvents( );
  }

  bool Context::CloseRequested( ) {
    return glfwWindowShouldClose(WND) != 0;
  }

  EngineException Context::CreateGraphicsException(std::string prefix) {
    throw EngineException(prefix + _lastErrorString, (ErrorCode) _lastErrorCode);
  }
}