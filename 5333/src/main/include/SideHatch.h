#pragma once

#include <string>

#include "strategy/Strategy.h"
#include "strategy/StrategySystem.h"
// #include "devices/DeployableDevice.h"
#include "actuators/BinaryActuator.h"
// #include "controllers/CurtinControllers.h"
// #include "Toggle.h"

// using SideHatchState = wml::devices::DeployableDeviceState;

// struct SideHatchConfig : public wml::devices::DeployableDeviceConfig {
//   wml::actuators::BinaryActuator &manipulator;
//   wml::actuators::BinaryActuatorState stowedState;

//   SideHatchConfig(wml::actuators::BinaryActuator &manipulatorIn, wml::actuators::BinaryActuator &actuatorIn, std::string name = "<Hatch Intake>", wml::actuators::BinaryActuatorState stowedStateIn = wml::actuators::kForward, bool canEjectIn = false) : wml::devices::DeployableDeviceConfig(actuatorIn, canEjectIn, name), stowedState(stowedStateIn), manipulator(manipulatorIn) {};
// };

// enum SideHatchState { kGrab, kWithdraw = 0, kReplace };

// class SideHatch : public wml::devices::StateDevice<>, public wml::StrategySystem {
//  public:
//   SideHatch(SideHatchConfig config) : DeployableDevice(config), _config(config) {};

//   SideHatchConfig &GetConfig() { return _config; };

//  protected:
//   virtual void IntakingPeriodic() override;  // Intake a hatch
//   virtual void OuttakingPeriodic() override; // Eject a hatch
//   virtual void StowedPeriodic() override;

//  private:
//   HatchISidentake _config;
// };
