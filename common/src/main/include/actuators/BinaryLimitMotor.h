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

      virtual devices::RawStateDevice *MakeRawStateDevice(std::string name = "<Binary Limit Motor>") override {
        return BinaryActuator::MakeRawStateDevice(name);
      }

      virtual void UpdateActuator(double dt) override;
      virtual void Stop() override;
      virtual bool IsDone() override;

     private:
      BinaryLimitMotorConfig _config;
    };
  } // ns actuators
} // ns curtinfrc
