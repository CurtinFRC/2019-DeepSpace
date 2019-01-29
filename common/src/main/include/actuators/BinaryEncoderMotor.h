#pragma once

#include "BinaryActuator.h"
#include "../Gearbox.h"

namespace curtinfrc {
  namespace actuators {
    struct BinaryEncoderMotorConfig {
      Gearbox &motor; // contains an encoder
      double forward, reverse; // the values for kForward and kReverse in (encoder) rotations

      double kp, ki, kd, kf; // PID constants
    };

    class BinaryEncoderMotor : public BinaryActuator {
     public:
      BinaryEncoderMotor(BinaryEncoderMotorConfig config, BinaryActuatorState initialState = kReverse) : _config(config), BinaryActuator(initialState) {};

      virtual void Update() override;
      virtual void Stop() override;
      virtual bool IsDone() override;

     private:
      BinaryEncoderMotorConfig _config;
    };
  } // ns actuators
} // ns curtinfrc
