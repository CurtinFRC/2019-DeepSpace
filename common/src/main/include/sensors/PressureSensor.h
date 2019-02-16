#pragma once

#include <frc/AnalogInput.h>

namespace curtinfrc {
  namespace sensors {
    class PressureSensor {
     public:
      PressureSensor(int channel);

      double Get();                        // %, assuming 120psi is 100%
      double GetPSI() { return Get() * 120; };

     private:
      frc::AnalogInput _sensor;
    };
  } // ns sensors
} // ns curtinfrc
