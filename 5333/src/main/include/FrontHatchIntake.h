#pragma once

#include <frc/Servo.h>

#include "strategy/Strategy.h"
#include "devices/DeployableDevice.h"
#include "CurtinControllers.h"
#include "Toggle.h"

using FrontHatchIntakeState = curtinfrc::devices::DeployableDeviceState;

struct FrontHatchIntakeConfig : public curtinfrc::devices::DeployableDeviceConfig {
  curtinfrc::actuators::BinaryActuator &ejector;
  bool ejectState;

  FrontHatchIntakeConfig(curtinfrc::actuators::BinaryActuator &ejectorIn, curtinfrc::actuators::BinaryActuator &actuatorIn, bool ejectStateIn) : curtinfrc::devices::DeployableDeviceConfig(actuatorIn), ejector(ejectorIn), ejectState(ejectStateIn){};
};

class FrontHatchIntake : public curtinfrc::devices::DeployableDevice {
 public:
  FrontHatchIntake(FrontHatchIntakeConfig config) : DeployableDevice(config), _config(config) {};

 protected:
  virtual void IntakingPeriodic() override;  // Intake a hatch
  virtual void OuttakingPeriodic() override; // Eject a hatch

 private:
  FrontHatchIntakeConfig _config;
};

class FrontHatchIntakeManualStrategy : public curtinfrc::Strategy {
 public:
  FrontHatchIntakeManualStrategy(FrontHatchIntake &frontHatchIntake, curtinfrc::Joystick &joy, bool startEnabled) : Strategy("Front Hatch Manual"),  _frontHatchIntake(frontHatchIntake), _joy(joy), _enabledToggle(curtinfrc::ONRISE), _enabled(startEnabled) {
    Requires(&frontHatchIntake);
    SetCanBeInterrupted(true);
    SetCanBeReused(true);
  }

  void OnUpdate(double dt) override;

 private:
  FrontHatchIntake &_frontHatchIntake;
  curtinfrc::Joystick &_joy;
  curtinfrc::Toggle _enabledToggle;
  bool _enabled;
};
