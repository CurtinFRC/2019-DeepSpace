#pragma once

#include <string>

#include "strategy/Strategy.h"
#include "strategy/StrategySystem.h"
#include "devices/DeployableDevice.h"
#include "actuators/BinaryActuator.h"
#include "controllers/CurtinControllers.h"
#include "Toggle.h"

using HatchIntakeState = curtinfrc::devices::DeployableDeviceState;

struct HatchIntakeConfig : public curtinfrc::devices::DeployableDeviceConfig {
  curtinfrc::actuators::BinaryActuator &manipulator;
  curtinfrc::actuators::BinaryActuatorState stowedState;

  HatchIntakeConfig(curtinfrc::actuators::BinaryActuator &manipulatorIn, curtinfrc::actuators::BinaryActuator &actuatorIn, std::string name = "<Hatch Intake>", curtinfrc::actuators::BinaryActuatorState stowedStateIn = curtinfrc::actuators::kForward, bool canEjectIn = false) : curtinfrc::devices::DeployableDeviceConfig(actuatorIn, canEjectIn, name), stowedState(stowedStateIn), manipulator(manipulatorIn) {};
};

class HatchIntake : public curtinfrc::devices::DeployableDevice, public curtinfrc::StrategySystem {
 public:
  HatchIntake(HatchIntakeConfig config) : DeployableDevice(config), _config(config) {};

  HatchIntakeConfig &GetConfig() { return _config; };

 protected:
  virtual void IntakingPeriodic() override;  // Intake a hatch
  virtual void OuttakingPeriodic() override; // Eject a hatch
  virtual void StowedPeriodic() override;

 private:
  HatchIntakeConfig _config;
};

class HatchIntakeManualStrategy : public curtinfrc::Strategy {
 public:
  HatchIntakeManualStrategy(HatchIntake &hatchIntake, curtinfrc::controllers::SmartControllerGroup &contGroup, bool startEnabled) : Strategy("Hatch Manual"),  _hatchIntake(hatchIntake), _contGroup(contGroup), _enabledToggle(curtinfrc::ONRISE), _enabled(startEnabled) {
    Requires(&hatchIntake);
    SetCanBeInterrupted(true);
    SetCanBeReused(true);

    // Make sure it's stowed!
    hatchIntake.SetStowed();
  }

  void OnUpdate(double dt) override;

 private:
  HatchIntake &_hatchIntake;
  curtinfrc::controllers::SmartControllerGroup &_contGroup;
  curtinfrc::Toggle _enabledToggle;
  bool _enabled;
};
