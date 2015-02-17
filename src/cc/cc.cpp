#include "stdafx.h"

#include <iostream>
#include <memory>

#include <cclib/fx/context.h>
#include <cclib/engineexception.h>
#include <cclib/fx/shaders.h>
#include <cclib/fx/spritebatch.h>

#include <gl/glew.h>
#include <gl/glfw3.h>

int main(int argc, char* argv[])
{
  try {
    auto context = std::make_shared<fx::Context>( );
    auto program = LoadShaders("shaders/vertex.glsl", "shaders/fragment.glsl");
    auto sb = std::make_shared<fx::SpriteBatch>( );
    auto y = 0;

    do {
      context->Begin( );
      glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
      glClear(GL_COLOR_BUFFER_BIT);
      glUseProgram(program);

      sb->Begin( );

      sb->Draw(0.1, (float)((y / 500.0f) - 1), 0, 0.1, 0.1);

      sb->End( );

      y = (y + 1) % 1000;

      context->End( );
    } while (!context->CloseRequested( ));

    return 0;
  } catch (EngineException ee) {
    std::cout << ee.what( ) << std::endl;
    return ee.code( );
  }
}

