#pragma once

#include <frc/AnalogInput.h>

#include "sensors/DoubleSensor.h"

namespace wml {
  namespace sensors {
    class PressureSensor : public DoubleSensor {
     public:
      PressureSensor(int channel, std::string name = "<Pressure Sensor>") : DoubleSensor(name, 120), _sensor(channel) {};

      virtual double Get();  // %, assuming 120psi is 100%

     private:
      frc::AnalogInput _sensor;
    };
  } // ns sensors
} // ns wml
