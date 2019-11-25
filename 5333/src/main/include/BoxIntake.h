#pragma once

#include <string>

#include "strategy/Strategy.h"
#include "strategy/StrategySystem.h"
#include "devices/DeployableDevice.h"
#include "controllers/CurtinControllers.h"
#include "Toggle.h"
#include "Gearbox.h"

using BoxIntakeState = wml::devices::DeployableDeviceState;

struct BoxIntakeConfig : public wml::devices::DeployableDeviceConfig {
  wml::Gearbox &motors;

  BoxIntakeConfig(wml::Gearbox &motorsIn, wml::actuators::BinaryActuator &actuatorIn, bool canEjectIn = true, std::string name = "<Box Intake>") : wml::devices::DeployableDeviceConfig(actuatorIn, canEjectIn, name), motors(motorsIn) {};
};

class BoxIntake : public wml::devices::DeployableDevice, public wml::StrategySystem {
 public:
  BoxIntake(BoxIntakeConfig config) : DeployableDevice(config), _config(config) {};

 protected:
  virtual void IntakingPeriodic() override;  // Intake a cargo
  virtual void OuttakingPeriodic() override; // Eject a cargo
  virtual void DeployingPeriodic() override;
  virtual void StowingPeriodic() override;
  virtual void StowedPeriodic() override;

 private:
  BoxIntakeConfig _config;
};

class BoxIntakeManualStrategy : public wml::Strategy {
 public:
  BoxIntakeManualStrategy(BoxIntake &boxIntake, wml::controllers::SmartControllerGroup &contGroup) : Strategy("Box Manual"),  _boxIntake(boxIntake), _contGroup(contGroup) {
    Requires(&boxIntake);
    SetCanBeInterrupted(true);
    SetCanBeReused(true);
  }

  void OnUpdate(double dt) override;

 private:
  BoxIntake &_boxIntake;
  wml::controllers::SmartControllerGroup &_contGroup;
};
