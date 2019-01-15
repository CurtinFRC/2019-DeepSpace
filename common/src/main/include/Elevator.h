#pragma once

#include "Gearbox.h"
#include "sensors/BinarySensor.h"

namespace curtinfrc {
  struct ElevatorConfig {
    curtinfrc::SensoredTransmission &spool;
    curtinfrc::sensors::BinarySensor *limitSensorTop;
    curtinfrc::sensors::BinarySensor *limitSensorBottom;
  };

  class Elevator {
   public:
    Elevator(curtinfrc::ElevatorConfig config) : _config(config) {};

    void Set(double power);

    private:
    curtinfrc::ElevatorConfig _config;
  };
} // ns curtinfrc
