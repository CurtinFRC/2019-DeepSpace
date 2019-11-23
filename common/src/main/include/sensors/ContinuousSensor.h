#pragma once

#include "sensors/SensorBase.h"

namespace curtinfrc {
  namespace sensors {
    class ContinuousSensor : public SensorBase<double> {
     public:
      ContinuousSensor(std::string name = "<Continuous Sensor>") : SensorBase(name) {};

      virtual double Get() = 0;
    };
  } // ns sensors
} // ns curtinfrc
