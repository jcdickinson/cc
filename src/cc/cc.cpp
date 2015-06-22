#include "stdafx.h"

#include <iostream>
#include <memory>

#include <cclib/fx/context.h>
#include <cclib/engineexception.h>
#include <cclib/fx/shaders.h>
#include <cclib/fx/spritebatch.h>
#include <cclib/math.h>
#include <cclib/content/contentmanager.h>
#include <cclib/fx/shader.h>
#include <cclib/fx/texture.h>

#include <gl/glew.h>
#include <gl/glfw3.h>

int main(int argc, char* argv[ ]) {
  try {
    auto cm = content::ContentManager(argv[0]);

    auto context = std::make_shared<fx::Context>( );
    auto shader = cm.LoadContent<fx::Shader>("shaders/sprite");
    auto texture = cm.LoadContent<fx::Texture>("textures/ball");

    auto sb = std::make_shared<fx::SpriteBatch>( );
    auto y = 0;

    auto matrix = math::mat_ortho(0, 640, 0, 480);
    shader->Uniform("MVP", matrix);

    auto v4 = math::vec4(1, 2, 3, 4);
    auto& z = v4.xyz( );
    printf("Something complex");
    z.x( ) = 5;

    do {
      context->Begin( );
      glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
      glClear(GL_COLOR_BUFFER_BIT);
      shader->Apply( );

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

      sb->Begin(math::mat_identity<4, 4>( ));

      sb->Draw((float)y, (float)y, 0.0f, 100.0f, 100.0f);

      sb->End( );

      y = (y + 1) % 400;

      context->End( );
    } while (!context->CloseRequested( ));

    return 0;
  } catch (EngineException ee) {
    std::cout << ee.what( ) << std::endl;
    return (int) ee.code( );
  }
}

