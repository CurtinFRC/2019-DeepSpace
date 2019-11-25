#pragma once

#include "sensors/SensorBase.h"

namespace wml {
  namespace sensors {
    class ContinuousSensor : public SensorBase<double> {
     public:
      ContinuousSensor(std::string name = "<Continuous Sensor>") : SensorBase(name) {};

      virtual double Get() = 0;
    };
  } // ns sensors
} // ns wml
