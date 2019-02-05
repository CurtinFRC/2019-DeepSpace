#pragma once

#include <frc/Servo.h>

#include "strategy/Strategy.h"
#include "devices/DeployableDevice.h"
#include "CurtinControllers.h"
#include "Toggle.h"

using BoxIntakeState = curtinfrc::devices::DeployableDeviceState;

struct BoxIntakeConfig : public curtinfrc::devices::DeployableDeviceConfig {
  frc::Servo &servo;
  int forward, reverse; // Servo position in degrees (forward => grab, reverse => eject)

  BoxIntakeConfig(frc::Servo &servoIn, curtinfrc::actuators::BinaryActuator &actuatorIn) : curtinfrc::devices::DeployableDeviceConfig(actuatorIn), servo(servoIn) {};
};

class BoxIntake : public curtinfrc::devices::DeployableDevice {
 public:
  BoxIntake(BoxIntakeConfig config) : DeployableDevice(config), _config(config) {};

 protected:
  virtual void IntakingPeriodic() override;  // Intake a box
  virtual void OuttakingPeriodic() override; // Eject a box

 private:
  BoxIntakeConfig _config;
};

class BoxIntakeManualStrategy : public curtinfrc::Strategy {
 public:
  BoxIntakeManualStrategy(BoxIntake &boxIntake, curtinfrc::Joystick &joy, bool startEnabled) : Strategy("Box Manual"),  _boxIntake(boxIntake), _joy(joy), _enabledToggle(curtinfrc::ONRISE), _enabled(startEnabled) {
    Requires(&boxIntake);
    SetCanBeInterrupted(true);
    SetCanBeReused(true);
  }

  void OnUpdate(double dt) override;

 private:
  BoxIntake &_boxIntake;
  curtinfrc::Joystick &_joy;
  curtinfrc::Toggle _enabledToggle;
  bool _enabled;
};
