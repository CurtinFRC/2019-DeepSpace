#pragma once

#include <string>

#include "strategy/Strategy.h"
#include "strategy/StrategySystem.h"
#include "devices/DeployableDevice.h"
#include "actuators/BinaryActuator.h"
#include "controllers/Controllers.h"
#include "Toggle.h"

using HatchIntakeState = wml::devices::DeployableDeviceState;

struct HatchIntakeConfig : public wml::devices::DeployableDeviceConfig {
  wml::actuators::BinaryActuator &manipulator;
  wml::actuators::BinaryActuatorState stowedState;

  HatchIntakeConfig(wml::actuators::BinaryActuator &manipulatorIn, wml::actuators::BinaryActuator &actuatorIn, std::string name = "<Hatch Intake>", wml::actuators::BinaryActuatorState stowedStateIn = wml::actuators::kForward, bool canEjectIn = false) : wml::devices::DeployableDeviceConfig(actuatorIn, canEjectIn, name), stowedState(stowedStateIn), manipulator(manipulatorIn) {};
};

class HatchIntake : public wml::devices::DeployableDevice, public wml::StrategySystem {
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

class HatchIntakeManualStrategy : public wml::Strategy {
 public:
  HatchIntakeManualStrategy(HatchIntake &hatchIntake, wml::controllers::SmartControllerGroup &contGroup, bool startEnabled) : Strategy("Hatch Manual"),  _hatchIntake(hatchIntake), _contGroup(contGroup), _enabledToggle(wml::ONRISE), _enabled(startEnabled) {
    Requires(&hatchIntake);
    SetCanBeInterrupted(true);
    SetCanBeReused(true);

    // Make sure it's stowed!
    hatchIntake.SetStowed();
  }

  void OnUpdate(double dt) override;

 private:
  HatchIntake &_hatchIntake;
  wml::controllers::SmartControllerGroup &_contGroup;
  wml::Toggle _enabledToggle;
  bool _enabled;
};
