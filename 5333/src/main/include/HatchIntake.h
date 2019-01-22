#pragma once

#include <frc/Servo.h>

#include "intakes/Intake.h"
#include "Gearbox.h"
#include "sensors/BinarySensor.h"

enum HatchIntakeState { kGrab, kEject, kAim, kHold };

struct HatchIntakeConfig {
  curtinfrc::Gearbox &pivot;
  curtinfrc::sensors::BinarySensor &limitSensorUp;
  curtinfrc::sensors::BinarySensor &limitSensorDown;
  frc::Servo &servo;
};

class HatchIntake : public curtinfrc::intakes::Intake<HatchIntakeState> {
 public:
  HatchIntake(HatchIntakeConfig config) : _config(config) {};

  void Set(HatchIntakeState state);

 protected:
  virtual void OnStateChange(HatchIntakeState newState, HatchIntakeState lastState) override;
  virtual void OnStatePeriodic(HatchIntakeState state, double dt) override;

 private:
  HatchIntakeConfig _config;
};
