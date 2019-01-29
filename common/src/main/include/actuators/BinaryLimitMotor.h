#pragma once

#include "BinaryActuator.h"
#include "../Gearbox.h"
#include "../sensors/BinarySensor.h"

namespace curtinfrc {
  namespace actuators {
    struct BinaryLimitMotorConfig {
      Gearbox &motor; // contains an encoder
      sensors::BinarySensor &forward, &reverse;
    };

    class BinaryLimitMotor : public BinaryActuator {
     public:
      BinaryLimitMotor(BinaryLimitMotorConfig config, BinaryActuatorState initialState = kReverse) : _config(config), BinaryActuator(initialState) {};

      virtual bool Actuate() override;
      virtual bool Done() override;

     private:
      BinaryLimitMotorConfig _config;
    };
  } // ns actuators
} // ns curtinfrc
