#pragma once

#include <string>

#include "BinaryActuator.h"
#include "../Gearbox.h"
#include "../sensors/BinarySensor.h"

namespace curtinfrc {
  namespace actuators {
    struct BinaryLimitMotorConfig {
      Gearbox &motor; // contains an encoder
      sensors::BinarySensor &forward, &reverse;

      std::string name = "<Binary Limit Motor>";
    };

    class BinaryLimitMotor : public BinaryActuator {
     public:
      BinaryLimitMotor(BinaryLimitMotorConfig config, BinaryActuatorState initialState = kReverse) : _config(config), BinaryActuator(config.name, initialState) {};

      virtual void UpdateActuator(double dt) override;
      virtual void Stop() override;
      virtual bool IsDone() override;

     private:
      BinaryLimitMotorConfig _config;
    };
  } // ns actuators
} // ns curtinfrc
