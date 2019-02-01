#pragma once

#include "devices/DeployableDevice.h"
#include "devices/StateDevice.h"
#include "Gearbox.h"
#include "CurtinControllers.h"

#include <frc/DoubleSolenoid.h>

using HarvesterIntakeState = curtinfrc::devices::DeployableDeviceState;

struct HarvesterIntakeConfig : public curtinfrc::devices::DeployableDeviceConfig {
  curtinfrc::Gearbox &motors;

  HarvesterIntakeConfig(curtinfrc::Gearbox &motorsIn, curtinfrc::actuators::BinaryActuator &actuatorIn) : curtinfrc::devices::DeployableDeviceConfig(actuatorIn), motors(motorsIn) {};
};

class HarvesterIntake : public curtinfrc::devices::DeployableDevice {
 public:
  HarvesterIntake(HarvesterIntakeConfig config) : DeployableDevice(config), _config(config) {};

 protected:
  virtual void IntakingPeriodic() override;
  virtual void OuttakingPeriodic() override;
  virtual void DeployingPeriodic() override;
  virtual void StowingPeriodic() override;
  virtual void StowedPeriodic() override;

 private:
  HarvesterIntakeConfig _config;
};

class HarvesterIntakeController {
 public: 
  HarvesterIntakeController(HarvesterIntake &harvesterIntake, curtinfrc::Joystick &joy) : _harvesterIntake(harvesterIntake), _joy(joy) {};
  void Update(double dt);

  private:
  HarvesterIntake &_harvesterIntake;
  curtinfrc::Joystick &_joy;
};


