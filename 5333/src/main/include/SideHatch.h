#pragma once

#include <string>

#include "strategy/Strategy.h"
#include "strategy/StrategySystem.h"
// #include "devices/DeployableDevice.h"
#include "actuators/BinaryActuator.h"
// #include "controllers/CurtinControllers.h"
// #include "Toggle.h"

// using SideHatchState = curtinfrc::devices::DeployableDeviceState;

// struct SideHatchConfig : public curtinfrc::devices::DeployableDeviceConfig {
//   curtinfrc::actuators::BinaryActuator &manipulator;
//   curtinfrc::actuators::BinaryActuatorState stowedState;

//   SideHatchConfig(curtinfrc::actuators::BinaryActuator &manipulatorIn, curtinfrc::actuators::BinaryActuator &actuatorIn, std::string name = "<Hatch Intake>", curtinfrc::actuators::BinaryActuatorState stowedStateIn = curtinfrc::actuators::kForward, bool canEjectIn = false) : curtinfrc::devices::DeployableDeviceConfig(actuatorIn, canEjectIn, name), stowedState(stowedStateIn), manipulator(manipulatorIn) {};
// };

// enum SideHatchState { kGrab, kWithdraw = 0, kReplace };

// class SideHatch : public curtinfrc::devices::StateDevice<>, public curtinfrc::StrategySystem {
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
