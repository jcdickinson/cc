#pragma once
#include <string>

#include "contextoptions.h"
#include "../engineexception.h"

namespace fx {

  class Context {
    public:
    Context(const Context&) = default;
    Context& operator=(const Context&) = delete;

    Context(const ContextOptions& options = ContextOptions( ), std::string title = "Game");
    ~Context( );

    void Begin( );
    void End( );

    bool CloseRequested( );

    static EngineException CreateGraphicsException(std::string prefix = "");

    private:
    const ContextOptions _options;
    void* _native;
  };

}