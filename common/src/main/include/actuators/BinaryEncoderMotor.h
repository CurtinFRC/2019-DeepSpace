#pragma once

#include <string>

#include "BinaryActuator.h"
#include "../Gearbox.h"

namespace curtinfrc {
  namespace actuators {
    struct BinaryEncoderMotorConfig {
      Gearbox &motor; // contains an encoder
      double forward, reverse; // the values for kForward and kReverse in (encoder) rotations

      std::string name = "<Binary Encoder Motor>";

      double kp = 0, ki = 0, kd = 0, kf = 0; // PID constants
    };

    class BinaryEncoderMotor : public BinaryActuator {
     public:
      BinaryEncoderMotor(BinaryEncoderMotorConfig config, BinaryActuatorState initialState = kReverse) : _config(config), BinaryActuator(config.name, initialState) {};

      virtual void UpdateActuator(double dt) override;
      virtual void Stop() override;
      virtual bool IsDone() override;

     private:
      BinaryEncoderMotorConfig _config;
    };
  } // ns actuators
} // ns curtinfrc
