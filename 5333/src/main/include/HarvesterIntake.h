#pragma once

#include "devices/DeployableDevice.h"
#include "devices/StateDevice.h"
#include "Gearbox.h"

#include "strategy/Strategy.h"
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

class HarvesterIntakeManualStrategy : public curtinfrc::Strategy {
 public: 
  HarvesterIntakeManualStrategy(HarvesterIntake &harvesterIntake, curtinfrc::ControllerGroup &contGroup) : Strategy("Harvester Manual"), _harvesterIntake(harvesterIntake), _contGroup(contGroup) {
    Requires(&harvesterIntake);
    SetCanBeInterrupted(true);
    SetCanBeReused(true);
  };

  void OnUpdate(double dt) override;

 private:
  HarvesterIntake &_harvesterIntake;
  curtinfrc::ControllerGroup &_contGroup;
};


