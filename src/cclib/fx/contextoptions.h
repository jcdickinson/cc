#pragma once
#include "adaptermode.h"

namespace fx {

  struct ContextOptions {

    const AdapterMode Mode;

    uint8_t AlphaBits, DepthBits, StencilBits;
    uint8_t Samples;
    
    bool DoubleBuffer;
    bool Debug;

    bool Resizeable;
    bool Visible;
    bool Decorated;
    bool Focused;
    bool AutoIconify;
    bool Topmost;

    ContextOptions(const AdapterMode& mode = AdapterMode( ))
      : Mode(mode) {

      AlphaBits = 8;
      DepthBits = 24;
      StencilBits = 8;
      Samples = 0;
      DoubleBuffer = true;
      Debug = false;

      Resizeable = false;
      Visible = true;
      Decorated = true;
      Focused = true;
      AutoIconify = true;
      Topmost = false;
    }

  };

}