#pragma once

#include <string>

#include <frc/DigitalInput.h>

#include "BinarySensor.h"

namespace wml {
  namespace sensors {
    class LimitSwitch : public BinarySensor {
     public:
      LimitSwitch(int channel, bool invert = false, std::string name = "<Limit Switch>");

      void SetInverted(bool invert);
      bool Get() override;

     private:
      frc::DigitalInput _switch;
      bool _inverted;
    };
  } // ns sensors
} // ns wml
