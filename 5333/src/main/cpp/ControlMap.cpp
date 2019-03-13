#include "ControlMap.h"

using namespace curtinfrc::controllers;


#if N_CONT == 2

void ControlMap::InitSmartControllerGroup(SmartControllerGroup &contGroup) {}

// DRIVETRAIN AXIS JOYMAP
const tAxis ControlMap::forwardAxis{ 1, Joystick::kYAxis };
const tAxis ControlMap::turnAxis{ 1, Joystick::kZAxis };
const tAxis ControlMap::forwardAxisFOC{ 1, Joystick::kYAxis };
const tAxis ControlMap::turnAxisFOC{ 1, Joystick::kXAxis };

const double ControlMap::axisDeadzone = 0.05;
const double ControlMap::axisDeadzoneFOC = 0.05;


// DRIVETRAIN JOYMAP
const std::vector<tButton> ControlMap::reverseDrivetrain{ { 1, 2 } };   // Toggles the direction of the drivetrain
const std::vector<tButton> ControlMap::holdMovement{ { 1, 9 } };        // Makes the robot 'line up' (0 magnitude but still rotating)
const std::vector<tButton> ControlMap::activateFOC{ { 1, 10 } };        // Toggles the drivetrain between Manual and FOC control


// BEELEVATOR JOYMAP
const std::vector<tButton> ControlMap::raiseLift{ { 2, 8 } };
const std::vector<tButton> ControlMap::lowerLift{ { 2, 7 } };

const std::vector<tButton> ControlMap::liftGoalGround{ { 2, 1 } }; //{ { 2, 1 } };
const std::vector<tButton> ControlMap::liftGoalLower1{ { 2, 9 } }; //{ { 2, 7 } };
const std::vector<tButton> ControlMap::liftGoalLower2{}; //{ { 2, 8 } };
const std::vector<tButton> ControlMap::liftGoalMiddle1{ { 2, 10 } }; //{ { 2, 9 } };
const std::vector<tButton> ControlMap::liftGoalMiddle2{}; //{ { 2, 10 } };
const std::vector<tButton> ControlMap::liftGoalUpper1{}; //{ { 2, 11 } };
const std::vector<tButton> ControlMap::liftGoalUpper2{}; //{ { 2, 12 } };

const double ControlMap::liftSetpointGround = 0;                                     // Exact values need to be tested
const double ControlMap::liftSetpointLower1 = 0.46;
const double ControlMap::liftSetpointLower2 = 0.68;
const double ControlMap::liftSetpointMiddle1 = 1.18;
const double ControlMap::liftSetpointMiddle2 = 1.39;
const double ControlMap::liftSetpointUpper1 = 1.89;
const double ControlMap::liftSetpointUpper2 = 1.7;

// HATCH JOYMAP
const std::vector<tButton> ControlMap::hatchGrab{ { 1, 3 }, { 2, 6 } };
const std::vector<tButton> ControlMap::hatchRelease{ { 1, 5 }, { 2, 4 } };
const std::vector<tButton> ControlMap::hatchStow{ { 2, 12 } };
const std::vector<tButton> ControlMap::hatchToggleEnabled{ { 1, 8 }, { 2, 1 } }; // Changes the currently enabled hatch intake



// BOX INTAKE JOYMAP
const std::vector<tButton> ControlMap::boxIn{ { 1, 4 }, { 2, 5 } };
const std::vector<tButton> ControlMap::boxOut{ { 1, 6 }, { 2, 3 } };
const std::vector<tButton> ControlMap::boxStow{ { 1, 11 }, { 2, 11 } };

const double ControlMap::boxDeployedThrottle = 0.6;
const double ControlMap::boxStowingThrottle = 0.4;
const double ControlMap::boxStowedThrottle = 0.2;


// CONTROLSYSTEM JOYMAP
const std::vector<tButton> ControlMap::compressorOn{ { 2, 2 } };

#elif N_CONT == 3

void ControlMap::InitSmartControllerGroup(SmartControllerGroup &contGroup) {
  const tButton incElevator = { 3, XboxController::kBumperLeft }, decElevator = { 3, XboxController::kBumperRight };
  std::vector<tButton> selectorElevator;
  for (int i = 0; i < 7; i++) selectorElevator.push_back({ 3, 20 + i });

  contGroup.GetController(incElevator.cont).Map({ incElevator, decElevator }, selectorElevator);


  const tAxis lowerElevator = { 3, XboxController::kLeftThrottle }, raiseElevator = { 3, XboxController::kRightThrottle };
  tButton lowerElevatorButton = { 3, 27 }, raiseElevatorButton = { 3, 28 };

  contGroup.GetController(lowerElevator.cont).Map(lowerElevator, lowerElevatorButton);
  contGroup.GetController(raiseElevator.cont).Map(raiseElevator, raiseElevatorButton);
}

// DRIVETRAIN AXIS JOYMAP
const tAxis ControlMap::forwardAxis{ 1, Joystick::kYAxis };
const tAxis ControlMap::turnAxis{ 1, Joystick::kZAxis };
const tAxis ControlMap::forwardAxisFOC{ 1, Joystick::kYAxis };
const tAxis ControlMap::turnAxisFOC{ 1, Joystick::kXAxis };

const double ControlMap::axisDeadzone = 0.05;
const double ControlMap::axisDeadzoneFOC = 0.05;


// DRIVETRAIN JOYMAP
const std::vector<tButton> ControlMap::reverseDrivetrain{ { 1, 2 } };   // Toggles the direction of the drivetrain
const std::vector<tButton> ControlMap::holdMovement{ { 1, 9 } };        // Makes the robot 'line up' (0 magnitude but still rotating)
const std::vector<tButton> ControlMap::activateFOC{ { 1, 10 } };        // Toggles the drivetrain between Manual and FOC control


// BEELEVATOR JOYMAP
const std::vector<tButton> ControlMap::raiseLift{ { 3, 27 } };
const std::vector<tButton> ControlMap::lowerLift{ { 3, 28 } };

const std::vector<tButton> ControlMap::liftGoalGround{ { 3, 20 } };
const std::vector<tButton> ControlMap::liftGoalLower1{ { 3, 21 } };
const std::vector<tButton> ControlMap::liftGoalLower2{ { 3, 22 } };
const std::vector<tButton> ControlMap::liftGoalMiddle1{ { 3, 23 } };
const std::vector<tButton> ControlMap::liftGoalMiddle2{ { 3, 24 } };
const std::vector<tButton> ControlMap::liftGoalUpper1{ { 3, 25 } };
const std::vector<tButton> ControlMap::liftGoalUpper2{ { 3, 26 } };

const double ControlMap::liftSetpointGround = 0;                                     // Exact values need to be tested
const double ControlMap::liftSetpointLower1 = 0.46;
const double ControlMap::liftSetpointLower2 = 0.68;
const double ControlMap::liftSetpointMiddle1 = 1.18;
const double ControlMap::liftSetpointMiddle2 = 1.39;
const double ControlMap::liftSetpointUpper1 = 1.89;
const double ControlMap::liftSetpointUpper2 = 1.7;

// HATCH JOYMAP
const std::vector<tButton> ControlMap::hatchGrab{ { 1, 3 }, { 3, XboxController::kA } };
const std::vector<tButton> ControlMap::hatchRelease{ { 1, 5 }, { 3, XboxController::kY } };
const std::vector<tButton> ControlMap::hatchStow{ { 3, XboxController::kStickRight } };
const std::vector<tButton> ControlMap::hatchToggleEnabled{ { 1, 8 }, { 3, XboxController::kStart } }; // Changes the currently enabled hatch intake



// BOX INTAKE JOYMAP
const std::vector<tButton> ControlMap::boxIn{ { 1, 4 }, { 3, XboxController::kB } };   // reversed
const std::vector<tButton> ControlMap::boxOut{ { 1, 6 }, { 3, XboxController::kX } };  // reversed
const std::vector<tButton> ControlMap::boxStow{ { 3, XboxController::kStickLeft } };

const double ControlMap::boxDeployedThrottle = 0.6;
const double ControlMap::boxStowingThrottle = 0.4;
const double ControlMap::boxStowedThrottle = 0.2;


// CONTROLSYSTEM JOYMAP
const std::vector<tButton> ControlMap::compressorOn{ { 3, XboxController::kBack } };

#endif
