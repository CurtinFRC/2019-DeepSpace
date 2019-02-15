#include "ControlMap.h"

using namespace curtinfrc;


// DRIVETRAIN AXIS JOYMAP
const double ControlMap::axisDeadzone = 0.05;
const double ControlMap::axisDeadzoneFOC = 0.05;


// DRIVETRAIN JOYMAP
const tJoymap ControlMap::reverseDrivetrain{ { 1, 2 }, nopair };   // Toggles the direction of the drivetrain
const tJoymap ControlMap::holdMovement{ { 1, 9 }, nopair };        // Makes the robot 'line up' (0 magnitude but still rotating)
const tJoymap ControlMap::activateFOC{ { 1, 10 }, nopair };        // Toggles the drivetrain between Manual and FOC control


// BEELEVATOR JOYMAP
const tJoymap ControlMap::raiseLift{ { 1, 8 }, nopair };
const tJoymap ControlMap::lowerLift{ { 1, 7 }, nopair };

const tJoymap ControlMap::liftGoalGround{ nopair, { 2, 1 } };
const tJoymap ControlMap::liftGoalLower1{ nopair, { 2, 7 } };
const tJoymap ControlMap::liftGoalLower2{ nopair, { 2, 8 } };
const tJoymap ControlMap::liftGoalMiddle1{ nopair, { 2, 9 } };
const tJoymap ControlMap::liftGoalMiddle2{ nopair, { 2, 10 } };
const tJoymap ControlMap::liftGoalUpper1{ nopair, { 2, 11 } };
const tJoymap ControlMap::liftGoalUpper2{ nopair, { 2, 12 } };

const double ControlMap::liftSetpointGround = 0;                                     // Exact values need to be tested
const double ControlMap::liftSetpointLower1 = 0.46;
const double ControlMap::liftSetpointLower2 = 0.68;
const double ControlMap::liftSetpointMiddle1 = 1.18;
const double ControlMap::liftSetpointMiddle2 = 1.39;
const double ControlMap::liftSetpointUpper1 = 1.89;
const double ControlMap::liftSetpointUpper2 = 2.10;


// HARVESTER JOYMAP
const tJoymap ControlMap::harveserIn{ { 1, 3 }, nopair };
const tJoymap ControlMap::harveserOut{ { 1, 5 }, nopair };
const tJoymap ControlMap::harveserStow{ { 1, 11 }, nopair };

const double ControlMap::harvesterThrottle = 1;


// HATCH JOYMAP
const tJoymap ControlMap::hatchGrab{ { 1, 4 }, nopair };
const tJoymap ControlMap::hatchRelease{ { 1, 6 }, nopair };
const tJoymap ControlMap::hatchStow{ { 1, 12 }, nopair };
const tJoymap ControlMap::hatchToggleEnabled{ { 1, 13 }, nopair }; // Changes the currently enabled hatch intake


// BOX INTAKE JOYMAP
const tJoymap ControlMap::boxIn{ { 1, 3 }, nopair };
const tJoymap ControlMap::boxOut{ { 1, 5 }, nopair };
const tJoymap ControlMap::boxStow{ { 1, 11 }, nopair };
