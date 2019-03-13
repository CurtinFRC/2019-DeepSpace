#pragma once

#include "sensors/SensorBase.h"

namespace curtinfrc {
  namespace sensors {
    class BinarySensor : public SensorBase<bool> {
     public:
      BinarySensor(std::string name = "<Binary Sensor>") : SensorBase(name) {};

      virtual bool Get() = 0;
    };
  } // ns sensors
} // ns curtinfrc
