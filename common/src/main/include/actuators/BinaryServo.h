#pragma once

#include <frc/Servo.h>

#include "BinaryActuator.h"

#define _EPS_ 1  // servo accuracy

namespace curtinfrc {
  namespace actuators {
    struct BinaryServoConfig {
      int servoPort;
      int forward, reverse; // forward and reverse positions, in servo degrees
    };

    class BinaryServo : public BinaryActuator, protected frc::Servo {
     public:
      BinaryServo(BinaryServoConfig config, BinaryActuatorState initialState = kReverse) : _config(config), BinaryActuator(initialState), frc::Servo(config.servoPort) {};

      virtual void Update(double dt) override;

     private:
      BinaryServoConfig _config;
    };
  } // ns actuators
} // ns curtinfrc
