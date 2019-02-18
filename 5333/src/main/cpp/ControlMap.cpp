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
const tJoymap ControlMap::raiseLift{ nopair, { 2, 8 } };
const tJoymap ControlMap::lowerLift{ nopair, { 2, 7 } };

const tJoymap ControlMap::liftGoalGround{ nopair, nopair }; //{ 2, 1 } };
const tJoymap ControlMap::liftGoalLower1{ nopair, nopair }; //{ 2, 7 } };
const tJoymap ControlMap::liftGoalLower2{ nopair, nopair }; //{ 2, 8 } };
const tJoymap ControlMap::liftGoalMiddle1{ nopair, nopair }; //{ 2, 9 } };
const tJoymap ControlMap::liftGoalMiddle2{ nopair, nopair }; //{ 2, 10 } };
const tJoymap ControlMap::liftGoalUpper1{ nopair, nopair }; //{ 2, 11 } };
const tJoymap ControlMap::liftGoalUpper2{ nopair, nopair }; //{ 2, 12 } };

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
const tJoymap ControlMap::hatchGrab{ { 1, 3 }, { 2, 6 } };
const tJoymap ControlMap::hatchRelease{ { 1, 5 }, { 2, 4 } };
const tJoymap ControlMap::hatchStow{ nopair, { 2, 12 } };
const tJoymap ControlMap::hatchToggleEnabled{ { 1, 8 }, { 2, 1 } }; // Changes the currently enabled hatch intake


// BOX INTAKE JOYMAP
const tJoymap ControlMap::boxIn{ { 1, 4 }, { 2, 5 } };
const tJoymap ControlMap::boxOut{ { 1, 6 }, { 2, 3 } };
const tJoymap ControlMap::boxStow{ nopair, { 2, 11 } };

const double ControlMap::boxDeployedThrottle = 0.6;
const double ControlMap::boxStowingThrottle = 0.4;
const double ControlMap::boxStowedThrottle = 0.2;


// CONTROLSYSTEM JOYMAP
const tJoymap ControlMap::compressorOn{ nopair, { 2, 2 } };
