#pragma once

namespace curtinfrc {
  namespace sensors {
    class BinarySensor {
     public:
      BinarySensor() {};

      virtual bool Get() = 0;
    };
  } // ns sensors
} // ns curtinfrc
