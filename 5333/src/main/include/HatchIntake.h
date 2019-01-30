#pragma once

#include <frc/Servo.h>

#include "devices/DeployableDevice.h"

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
  virtual void DeployedPeriodic(HatchIntakeState state) final {
    if (state == HatchIntakeState::kIntaking) IntakingPeriodic(); else OuttakingPeriodic();
  };

  virtual void IntakingPeriodic();  // Intake a hatch
  virtual void OuttakingPeriodic(); // Eject a hatch

 private:
  HatchIntakeConfig _config;
};
