#pragma once

#include <frc/DigitalInput.h>

#include "BinarySensor.h"

namespace curtinfrc {
  namespace sensors {
    class LimitSwitch : public BinarySensor {
     public:
      LimitSwitch(int channel);

      bool Get() override;

     private:
      frc::DigitalInput *_switch;
    };
  } // ns sensors
} // ns curtinfrc
