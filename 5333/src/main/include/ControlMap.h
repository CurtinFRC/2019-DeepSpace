#pragma once

#include "CurtinControllers.h"

class ControlMap {
 public:
  // DRIVETRAIN AXIS JOYMAP
  static const curtinfrc::Joystick::AxisType forwardAxis = curtinfrc::Joystick::kYAxis;
  static const curtinfrc::Joystick::AxisType turnAxis = curtinfrc::Joystick::kZAxis;
  static const curtinfrc::Joystick::AxisType forwardAxisFOC = curtinfrc::Joystick::kYAxis;
  static const curtinfrc::Joystick::AxisType turnAxisFOC = curtinfrc::Joystick::kXAxis;

  static const double axisDeadzone;
  static const double axisDeadzoneFOC;


  // DRIVETRAIN JOYMAP
  static const curtinfrc::tJoymap reverseDrivetrain;   // Toggles the direction of the drivetrain
  static const curtinfrc::tJoymap holdMovement;        // Makes the robot 'line up' (0 magnitude but still rotating)
  static const curtinfrc::tJoymap activateFOC;        // Toggles the drivetrain between Manual and FOC control


  // BEELEVATOR JOYMAP
  static const curtinfrc::tJoymap raiseLift;
  static const curtinfrc::tJoymap lowerLift;

  static const int goalGround = 1; // Gonna have conflicting buttons for now, but oh well *
  static const int goalLower1 = 7;
  static const int goalLower2 = 8;
  static const int goalMiddle1 = 9;
  static const int goalMiddle2 = 10;
  static const int goalUpper1 = 11;
  static const int goalUpper2 = 12;

  static constexpr double setpointGround = 0; // Exact values need to be tested *
  static constexpr double setpointLower1 = 0.46;
  static constexpr double setpointLower2 = 0.68;
  static constexpr double setpointMiddle1 = 1.18;
  static constexpr double setpointMiddle2 = 1.39;
  static constexpr double setpointUpper1 = 1.89;
  static constexpr double setpointUpper2 = 2.10;


  // HARVESTER JOYMAP
  static const curtinfrc::tJoymap harveserIn;
  static const curtinfrc::tJoymap harveserOut;
  static const curtinfrc::tJoymap harveserStow;

  static const double harvesterThrottle;


  // HATCH JOYMAP
  static const curtinfrc::tJoymap hatchGrab;
  static const curtinfrc::tJoymap hatchRelease;
  static const curtinfrc::tJoymap hatchStow;
  static const curtinfrc::tJoymap hatchToggleEnabled; // Changes the currently enabled hatch intake


  // BOX INTAKE JOYMAP
  static const curtinfrc::tJoymap boxIn;
  static const curtinfrc::tJoymap boxOut;
  static const curtinfrc::tJoymap boxStow;
};
