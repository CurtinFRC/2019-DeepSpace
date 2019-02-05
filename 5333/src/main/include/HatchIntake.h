#pragma once

#include <frc/Servo.h>

#include "strategy/Strategy.h"
#include "devices/DeployableDevice.h"
#include "CurtinControllers.h"
#include "Toggle.h"

using HatchIntakeState = curtinfrc::devices::DeployableDeviceState;

struct HatchIntakeConfig : public curtinfrc::devices::DeployableDeviceConfig {
  frc::Servo &servo;
  int forward, reverse; // Servo position in degrees (forward => grab, reverse => eject)

  HatchIntakeConfig(frc::Servo &servoIn, curtinfrc::actuators::BinaryActuator &actuatorIn) : curtinfrc::devices::DeployableDeviceConfig(actuatorIn), servo(servoIn) {};
};

class HatchIntake : public curtinfrc::devices::DeployableDevice {
 public:
  HatchIntake(HatchIntakeConfig config) : DeployableDevice(config), _config(config) {};

 protected:
  virtual void IntakingPeriodic() override;  // Intake a hatch
  virtual void OuttakingPeriodic() override; // Eject a hatch

 private:
  HatchIntakeConfig _config;
};

class HatchIntakeManualStrategy : public curtinfrc::Strategy {
 public:
  HatchIntakeManualStrategy(HatchIntake &hatchIntake, curtinfrc::Joystick &joy, bool startEnabled) : Strategy("Hatch Manual"),  _hatchIntake(hatchIntake), _joy(joy), _enabledToggle(curtinfrc::ONRISE), _enabled(startEnabled) {
    Requires(&hatchIntake);
    SetCanBeInterrupted(true);
    SetCanBeReused(true);
  }

  void OnUpdate(double dt) override;

 private:
  HatchIntake &_hatchIntake;
  curtinfrc::Joystick &_joy;
  curtinfrc::Toggle _enabledToggle;
  bool _enabled;
};
