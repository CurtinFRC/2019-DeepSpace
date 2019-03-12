#pragma once

#include "sensors/SensorBase.h"

namespace curtinfrc {
  namespace sensors {
    class DoubleSensor : public SensorBase<double> {
     public:
      DoubleSensor(std::string name = "<Double Sensor>", double max = 1, double min = 0) : SensorBase(name), _max(max), _min(min) {};

      virtual double Get() = 0;          // returns from 0 to 1
      double GetScaled() { return Get() * (std::abs(_max) + std::abs(_min)) + _min; };

     private:
      const double _max, _min;
    };
  } // ns sensors
} // ns curtinfrc
