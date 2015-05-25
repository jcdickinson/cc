#pragma once

namespace fx {

  class IGpuState {
    public:
    virtual void Apply( ) = 0;
  };

}