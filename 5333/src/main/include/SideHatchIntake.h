#pragma once

#include <frc/Servo.h>

#include "strategy/Strategy.h"
#include "devices/DeployableDevice.h"
#include "CurtinControllers.h"
#include "Toggle.h"

using SideHatchIntakeState = curtinfrc::devices::DeployableDeviceState;

struct SideHatchIntakeConfig : public curtinfrc::devices::DeployableDeviceConfig {
  frc::Servo &servo;
  int forward, reverse; // Servo position in degrees (forward => grab, reverse => eject)

  SideHatchIntakeConfig(frc::Servo &servoIn, curtinfrc::actuators::BinaryActuator &actuatorIn, int forwardIn, int reverseIn) : curtinfrc::devices::DeployableDeviceConfig(actuatorIn), servo(servoIn), forward(forwardIn), reverse(reverseIn) {};
};

class SideHatchIntake : public curtinfrc::devices::DeployableDevice {
 public:
  SideHatchIntake(SideHatchIntakeConfig config) : DeployableDevice(config), _config(config) {};

 protected:
  virtual void IntakingPeriodic() override;  // Intake a hatch
  virtual void OuttakingPeriodic() override; // Eject a hatch

 private:
  SideHatchIntakeConfig _config;
};

class SideHatchIntakeManualStrategy : public curtinfrc::Strategy {
 public:
  SideHatchIntakeManualStrategy(SideHatchIntake &sideHatchIntake, curtinfrc::Joystick &joy, bool startEnabled) : Strategy("Side Hatch Manual"),  _sideHatchIntake(sideHatchIntake), _joy(joy), _enabledToggle(curtinfrc::ONRISE), _enabled(startEnabled) {
    Requires(&sideHatchIntake);
    SetCanBeInterrupted(true);
    SetCanBeReused(true);
  }

  void OnUpdate(double dt) override;

 private:
  SideHatchIntake &_sideHatchIntake;
  curtinfrc::Joystick &_joy;
  curtinfrc::Toggle _enabledToggle;
  bool _enabled;
};
