#pragma once

#include <frc/Servo.h>

#include "intakes/DeployableIntake.h"
#include "CurtinControllers.h"
#include "Toggle.h"

using HatchIntakeState = curtinfrc::intakes::DeployableIntakeState;

struct HatchIntakeConfig : public curtinfrc::intakes::DeployableIntakeConfig {
  frc::Servo &servo;
  int forward, reverse; // Servo position in degrees (forward => grab, reverse => eject)

  HatchIntakeConfig(frc::Servo &servoIn, curtinfrc::actuators::BinaryActuator &actuatorIn) : curtinfrc::intakes::DeployableIntakeConfig(actuatorIn), servo(servoIn) {};
};

class HatchIntake : public curtinfrc::intakes::DeployableIntake {
 public:
  HatchIntake(HatchIntakeConfig config) : DeployableIntake(config), _config(config) {};

 protected:
  virtual void DeployedPeriodic(HatchIntakeState state) final {
    if (state == HatchIntakeState::kIntaking) IntakingPeriodic(); else OuttakingPeriodic();
  };

  virtual void IntakingPeriodic();  // Intake a hatch
  virtual void OuttakingPeriodic(); // Eject a hatch

 private:
  HatchIntakeConfig _config;
};

class HatchIntakeController {
 public:
  HatchIntakeController(HatchIntake &hatchIntake, curtinfrc::Joystick &joy, bool startEnabled) : _hatchIntake(hatchIntake), _joy(joy), _enabledToggle(curtinfrc::ONRISE), _enabled(startEnabled) {};
  void Update(double dt);

 private:
  HatchIntake &_hatchIntake;
  curtinfrc::Joystick &_joy;
  curtinfrc::Toggle _enabledToggle;
  bool _enabled;
};
