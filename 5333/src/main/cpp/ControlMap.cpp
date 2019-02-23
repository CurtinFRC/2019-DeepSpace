#include "ControlMap.h"

using namespace curtinfrc;


#if N_CONT == 2

// DRIVETRAIN AXIS JOYMAP
const tControllerAxis ControlMap::forwardAxis{ 1, curtinfrc::Joystick::JoyAxis::kYAxis };
const tControllerAxis ControlMap::turnAxis{ 1, curtinfrc::Joystick::JoyAxis::kZAxis };
const tControllerAxis ControlMap::forwardAxisFOC{ 1, curtinfrc::Joystick::JoyAxis::kYAxis };
const tControllerAxis ControlMap::turnAxisFOC{ 1, curtinfrc::Joystick::JoyAxis::kXAxis };

const double ControlMap::axisDeadzone = 0.05;
const double ControlMap::axisDeadzoneFOC = 0.05;


// DRIVETRAIN JOYMAP
const tControllerButtonMap ControlMap::reverseDrivetrain{ { 1, 2 } };   // Toggles the direction of the drivetrain
const tControllerButtonMap ControlMap::holdMovement{ { 1, 9 } };        // Makes the robot 'line up' (0 magnitude but still rotating)
const tControllerButtonMap ControlMap::activateFOC{ { 1, 10 } };        // Toggles the drivetrain between Manual and FOC control


// BEELEVATOR JOYMAP
const tControllerButtonMap ControlMap::raiseLift{ { 2, 8 } };
const tControllerButtonMap ControlMap::lowerLift{ { 2, 7 } };

const tControllerButtonMap ControlMap::liftGoalGround{ { 2, 1 } }; //{ { 2, 1 } };
const tControllerButtonMap ControlMap::liftGoalLower1{ { 2, 9 } }; //{ { 2, 7 } };
const tControllerButtonMap ControlMap::liftGoalLower2{}; //{ { 2, 8 } };
const tControllerButtonMap ControlMap::liftGoalMiddle1{ { 2, 10 } }; //{ { 2, 9 } };
const tControllerButtonMap ControlMap::liftGoalMiddle2{}; //{ { 2, 10 } };
const tControllerButtonMap ControlMap::liftGoalUpper1{}; //{ { 2, 11 } };
const tControllerButtonMap ControlMap::liftGoalUpper2{}; //{ { 2, 12 } };

const double ControlMap::liftSetpointGround = 0;                                     // Exact values need to be tested
const double ControlMap::liftSetpointLower1 = 0.46;
const double ControlMap::liftSetpointLower2 = 0.68;
const double ControlMap::liftSetpointMiddle1 = 1.18;
const double ControlMap::liftSetpointMiddle2 = 1.39;
const double ControlMap::liftSetpointUpper1 = 1.89;
const double ControlMap::liftSetpointUpper2 = 1.7;

// HATCH JOYMAP
const tControllerButtonMap ControlMap::hatchGrab{ { 1, 3 }, { 2, 6 } };
const tControllerButtonMap ControlMap::hatchRelease{ { 1, 5 }, { 2, 4 } };
const tControllerButtonMap ControlMap::hatchStow{ { 2, 12 } };
const tControllerButtonMap ControlMap::hatchToggleEnabled{ { 1, 8 }, { 2, 1 } }; // Changes the currently enabled hatch intake



// BOX INTAKE JOYMAP
const tControllerButtonMap ControlMap::boxIn{ { 1, 4 }, { 2, 5 } };
const tControllerButtonMap ControlMap::boxOut{ { 1, 6 }, { 2, 3 } };
const tControllerButtonMap ControlMap::boxStow{ { 1, 11 }, { 2, 11 } };

const double ControlMap::boxDeployedThrottle = 0.6;
const double ControlMap::boxStowingThrottle = 0.4;
const double ControlMap::boxStowedThrottle = 0.2;


// CONTROLSYSTEM JOYMAP
const tControllerButtonMap ControlMap::compressorOn{ { 2, 2 } };

#elif N_CONT == 3

// DRIVETRAIN AXIS JOYMAP
const tControllerAxis ControlMap::forwardAxis{ 1, curtinfrc::Joystick::JoyAxis::kYAxis };
const tControllerAxis ControlMap::turnAxis{ 1, curtinfrc::Joystick::JoyAxis::kZAxis };
const tControllerAxis ControlMap::forwardAxisFOC{ 1, curtinfrc::Joystick::JoyAxis::kYAxis };
const tControllerAxis ControlMap::turnAxisFOC{ 1, curtinfrc::Joystick::JoyAxis::kXAxis };

const double ControlMap::axisDeadzone = 0.05;
const double ControlMap::axisDeadzoneFOC = 0.05;


// DRIVETRAIN JOYMAP
const tControllerButtonMap ControlMap::reverseDrivetrain{ { 1, 2 } };   // Toggles the direction of the drivetrain
const tControllerButtonMap ControlMap::holdMovement{ { 1, 9 } };        // Makes the robot 'line up' (0 magnitude but still rotating)
const tControllerButtonMap ControlMap::activateFOC{ { 1, 10 } };        // Toggles the drivetrain between Manual and FOC control


// BEELEVATOR JOYMAP
const tControllerButtonMap ControlMap::raiseLift{};
const tControllerButtonMap ControlMap::lowerLift{};

const tControllerSelectorMapping ControlMap::liftSelectorConfig{
  { 3, XboxController::kBumperLeft }, { 3, XboxController::kBumperRight },
  7, 0, ControlMap::liftSelectorID
};

const tControllerSelectorButtonMap ControlMap::liftGoalGround{ { 3, 1, ControlMap::liftSelectorID } };
const tControllerSelectorButtonMap ControlMap::liftGoalLower1{ { 3, 7, ControlMap::liftSelectorID } };
const tControllerSelectorButtonMap ControlMap::liftGoalLower2{ { 3, 8, ControlMap::liftSelectorID }};
const tControllerSelectorButtonMap ControlMap::liftGoalMiddle1{ { 3, 9, ControlMap::liftSelectorID } };
const tControllerSelectorButtonMap ControlMap::liftGoalMiddle2{ { 3, 10, ControlMap::liftSelectorID } }; 
const tControllerSelectorButtonMap ControlMap::liftGoalUpper1{ { 3, 11, ControlMap::liftSelectorID } }; 
const tControllerSelectorButtonMap ControlMap::liftGoalUpper2{ { 3, 12, ControlMap::liftSelectorID } }; 

const double ControlMap::liftSetpointGround = 0;                                     // Exact values need to be tested
const double ControlMap::liftSetpointLower1 = 0.46;
const double ControlMap::liftSetpointLower2 = 0.68;
const double ControlMap::liftSetpointMiddle1 = 1.18;
const double ControlMap::liftSetpointMiddle2 = 1.39;
const double ControlMap::liftSetpointUpper1 = 1.89;
const double ControlMap::liftSetpointUpper2 = 1.7;

// HATCH JOYMAP
const tControllerButtonMap ControlMap::hatchGrab{ { 1, 3 }, { 2, 6 }, { 3, XboxController::kA } };
const tControllerButtonMap ControlMap::hatchRelease{ { 1, 5 }, { 2, 4 }, { 3, XboxController::kY } };
const tControllerButtonMap ControlMap::hatchStow{}; //{ { 3, 6 } };
const tControllerButtonMap ControlMap::hatchToggleEnabled{ { 3, XboxController::kStickRight } }; // Changes the currently enabled hatch intake



// BOX INTAKE JOYMAP
const tControllerButtonMap ControlMap::boxIn{ { 1, 4 }, { 2, 5 }, { 3, 2 } };   // reversed
const tControllerButtonMap ControlMap::boxOut{ { 1, 6 }, { 2, 3 }, { 3, 3 } };  // reversed
const tControllerButtonMap ControlMap::boxStow{ { 3, 5 } };

const double ControlMap::boxDeployedThrottle = 0.6;
const double ControlMap::boxStowingThrottle = 0.4;
const double ControlMap::boxStowedThrottle = 0.2;


// CONTROLSYSTEM JOYMAP
const tControllerButtonMap ControlMap::compressorOn{ { 2, 2 }, { 3, 7 } };

#endif
