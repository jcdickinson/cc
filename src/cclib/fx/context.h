#pragma once
#include <string>

#include "contextoptions.h"
#include "../engineexception.h"

namespace fx {

  class Context {
    private:
    const ContextOptions _options;
    void* _native;

    public:
    Context(const ContextOptions& options = ContextOptions( ), std::string title = "Game");
    ~Context( );

    void Begin( );
    void End( );

    bool CloseRequested( );

    static EngineException CreateGraphicsException(std::string prefix = "");
  };

}