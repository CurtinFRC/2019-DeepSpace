#pragma once

#include <frc/Servo.h>

#include "StateDevice.h"
#include "Gearbox.h"
#include "sensors/BinarySensor.h"

enum HatchIntakeState { kGrab, kAim, kHold, kEject, kStow  };

struct HatchIntakeConfig {
  curtinfrc::Gearbox &pivot;
  curtinfrc::sensors::BinarySensor &limitSensorUp;
  curtinfrc::sensors::BinarySensor &limitSensorDown;
  frc::Servo &servo;
};

class HatchIntake : public curtinfrc::StateDevice<HatchIntakeState> {
 public:
  HatchIntake(HatchIntakeConfig config) : _config(config) {};

 protected:
  virtual void OnStatePeriodic(HatchIntakeState state, double dt) override;

 private:
  HatchIntakeConfig _config;
};
