#pragma once

#include <string>

namespace curtinfrc {
  namespace sensors {
    template <typename returnType>
    class SensorBase {
     public:
      SensorBase(std::string name = "<Sensor Base>") : _name(name) {};

      virtual returnType Get() = 0;

      std::string GetName() { return _name; };

     private:
      std::string _name;
    };
  }
}
