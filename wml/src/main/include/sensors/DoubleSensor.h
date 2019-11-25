#pragma once

#include "sensors/ContinuousSensor.h"

namespace wml {
  namespace sensors {
    class DoubleSensor : public ContinuousSensor {
     public:
      DoubleSensor(std::string name = "<Double Sensor>", double max = 1, double min = 0) : ContinuousSensor(name), _max(max), _min(min) {};

      virtual double Get() = 0;          // should return from _min -> 0, _max -> _max
      double GetScaled() { return Get() * (std::abs(_max) + std::abs(_min)) + _min; };

     private:
      const double _max, _min;
    };
  } // ns sensors
} // ns wml
