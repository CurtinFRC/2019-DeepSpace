#include "ControlMap.h"

using namespace wml::controllers;


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
const std::vector<tButton> ControlMap::holdMovement{ { 1, 3 } };        // Makes the robot 'line up' (0 magnitude but still rotating)
const std::vector<tButton> ControlMap::activateFOC{ { 1, 5 } };        // Toggles the drivetrain between Manual and FOC control


// BEELEVATOR JOYMAP
const std::vector<tButton> ControlMap::raiseLift{ { 1, 6 } };
const std::vector<tButton> ControlMap::lowerLift{ { 1, 4 } };

const std::vector<tButton> ControlMap::liftGoalGround{ { 1, 1 } }; //{ { 2, 1 } };
const std::vector<tButton> ControlMap::liftGoalLower1{ { 1, 11 } }; //{ { 2, 7 } };
const std::vector<tButton> ControlMap::liftGoalLower2{ { 1, 12 }}; //{ { 2, 8 } };
const std::vector<tButton> ControlMap::liftGoalMiddle1{ { 1, 9 } }; //{ { 2, 9 } };
const std::vector<tButton> ControlMap::liftGoalMiddle2{ { 1, 10 } }; //{ { 2, 10 } };
const std::vector<tButton> ControlMap::liftGoalUpper1{ { 1, 7 } }; //{ { 2, 11 } };
const std::vector<tButton> ControlMap::liftGoalUpper2{ { 1, 8 } }; //{ { 2, 12 } };

const double ControlMap::liftSetpointGround = 0;                                     // Exact values need to be tested
const double ControlMap::liftSetpointLower1 = 0.46;
const double ControlMap::liftSetpointLower2 = 0.68;
const double ControlMap::liftSetpointMiddle1 = 1.18;
const double ControlMap::liftSetpointMiddle2 = 1.39;
const double ControlMap::liftSetpointUpper1 = 1.89;
const double ControlMap::liftSetpointUpper2 = 1.7;

// HATCH JOYMAP
const std::vector<tButton> ControlMap::hatchGrab{ { 2, 3 } };
const std::vector<tButton> ControlMap::hatchRelease{ { 2, 5 } };
const std::vector<tButton> ControlMap::hatchStow{ { 2, 2 } };
// const std::vector<tButton> ControlMap::hatchToggleEnabled{ { 1, 8 }, { 2, 1 } }; // Changes the currently enabled hatch intake



// BOX INTAKE JOYMAP
const std::vector<tButton> ControlMap::boxIn{ { 2, 4 } };
const std::vector<tButton> ControlMap::boxOut{ { 2, 6 } };
const std::vector<tButton> ControlMap::boxStow{ { 2, 2 } };

const double ControlMap::boxDeployedThrottle = 0.6;
const double ControlMap::boxStowingThrottle = 0.4;
const double ControlMap::boxStowedThrottle = 0.2;


// CONTROLSYSTEM JOYMAP
// const std::vector<tButton> ControlMap::compressorOn{ { 2, 2 } };

#elif N_CONT == 3

void ControlMap::InitSmartControllerGroup(SmartControllerGroup &contGroup) {
  // Joystick (1) Rebinds
  const tPOV buttonPOV = { 1, 0 };
  std::vector<tButton> povButtons;
  for (int i = 0; i < 8; i++) povButtons.push_back(tButton({ buttonPOV.cont, 30 + i }));

  contGroup.GetController(buttonPOV.cont).Map(buttonPOV, povButtons);

  // Joystick (2) Rebinds
  const tAxis manualElevator = { 2, Joystick::kYAxis };
  std::vector<tButton> manualElevatorPair = { { manualElevator.cont, 20 }, { manualElevator.cont, 21 } };

  contGroup.GetController(manualElevator.cont).Map(manualElevator, manualElevatorPair); // note, as Y is inverted, forwards is down


  // XBOX Rebinds
  // const tButton incElevator = { 3, XboxController::kBumperLeft }, decElevator = { 3, XboxController::kBumperRight };
  // std::vector<tButton> selectorElevator;
  // for (int i = 0; i < 7; i++) selectorElevator.push_back({ 3, 20 + i });

  // contGroup.GetController(incElevator.cont).Map({ incElevator, decElevator }, selectorElevator);


  const tAxis lowerElevator = { 3, XboxController::kLeftThrottle }, raiseElevator = { 3, XboxController::kRightThrottle };
  tButton lowerElevatorButton = { 3, 28 }, raiseElevatorButton = { 3, 27 };

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
const std::vector<tButton> ControlMap::holdMovement{ { 1, 3 } };        // Makes the robot 'line up' (0 magnitude but still rotating)
const std::vector<tButton> ControlMap::activateFOC{ { 1, 5 } };        // Toggles the drivetrain between Manual and FOC control
const tAxis ControlMap::drivetrainThrottle{ 1, Joystick::kThrottleAxis };
const std::vector<tButton> ControlMap::chargeForward{ { 1, 30 } };

const double ControlMap::drivetrainMinThrottle       = 0.3;
const double ControlMap::drivetrainForwardThrottle   = 0.9;
const double ControlMap::drivetrainTurnThrottle      = 0.6;

// BEELEVATOR JOYMAP
const std::vector<tButton> ControlMap::raiseLift{ { 1, 6 }, { 2, 20 }, { 3, 27 } };
const std::vector<tButton> ControlMap::lowerLift{ { 1, 4 }, { 2, 21 } };

const std::vector<tButton> ControlMap::liftGoalGround{ { 1, 1 }, { 3, 28 } };
const std::vector<tButton> ControlMap::liftGoalLower1{ { 1, 11 } };
const std::vector<tButton> ControlMap::liftGoalLower2{ { 1, 12 } };
const std::vector<tButton> ControlMap::liftGoalMiddle1{ { 1, 9 } };
const std::vector<tButton> ControlMap::liftGoalMiddle2{ { 1, 10 } };
const std::vector<tButton> ControlMap::liftGoalUpper1{ { 1, 7 } };
const std::vector<tButton> ControlMap::liftGoalUpper2{ { 1, 8 } };

const double ControlMap::liftSetpointGround = 0;                                     // Exact values need to be tested
const double ControlMap::liftSetpointLower1 = 0.2;
const double ControlMap::liftSetpointLower2 = 0.3;
const double ControlMap::liftSetpointMiddle1 = 1.01;
const double ControlMap::liftSetpointMiddle2 = 1.1;
const double ControlMap::liftSetpointUpper1 = 1.63;
const double ControlMap::liftSetpointUpper2 = 1.78;

// HATCH JOYMAP
const std::vector<tButton> ControlMap::hatchGrab{ { 2, 3 }, { 3, XboxController::kY } };
const std::vector<tButton> ControlMap::hatchRelease{ { 2, 5 }, { 3, XboxController::kA } };
const std::vector<tButton> ControlMap::hatchStow{ { 2, 2 }, { 3, XboxController::kBumperRight } };
const std::vector<tButton> ControlMap::hatchToggleEnabled{ { 2, 1 }, { 3, XboxController::kStart } }; // Changes the currently enabled hatch intake



// BOX INTAKE JOYMAP
const std::vector<tButton> ControlMap::boxIn{ { 2, 4 }, { 3, XboxController::kX } };   // reversed
const std::vector<tButton> ControlMap::boxOut{ { 2, 6 }, { 3, XboxController::kB } };  // reversed
const std::vector<tButton> ControlMap::boxStow{ { 2, 1 }, { 3, XboxController::kBumperLeft } };

const double ControlMap::boxDeployedThrottle = 0.6;
const double ControlMap::boxStowingThrottle = 0.4;
const double ControlMap::boxStowedThrottle = 0.2;


// CONTROLSYSTEM JOYMAP
// const std::vector<tButton> ControlMap::compressorOn{ { 3, XboxController::kBack } };

#endif
