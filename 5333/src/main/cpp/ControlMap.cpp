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
