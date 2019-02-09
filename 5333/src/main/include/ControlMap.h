#pragma once

#include "CurtinControllers.h"

class ControlMap {
 public:
  // DRIVETRAIN AXIS JOYMAP
  static const curtinfrc::Joystick::AxisType forwardAxis = curtinfrc::Joystick::kYAxis;
  static const curtinfrc::Joystick::AxisType turnAxis = curtinfrc::Joystick::kZAxis;
  static const curtinfrc::Joystick::AxisType forwardAxisFOC = curtinfrc::Joystick::kYAxis;
  static const curtinfrc::Joystick::AxisType turnAxisFOC = curtinfrc::Joystick::kXAxis;

  static constexpr double axisDeadzone = 0.05;
  static constexpr double axisDeadzoneFOC = 0.05;


  // DRIVETRAIN JOYMAP
  static const int reverseDrivetrain = 2;   // Toggles the direction of the drivetrain
  static const int holdMovement = 9;        // Makes the robot 'line up' (0 magnitude but still rotating)
  static const int activateFOC = 10;        // Toggles the drivetrain between Manual and FOC control


  // BEELEVATOR JOYMAP
  static const int raiseLift = 8;
  static const int lowerLift = 7;

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
  static const int harveserIn = 3;
  static const int harveserOut = 5;
  static const int harveserStow = 11;

  static constexpr double harvesterThrottle = 1;


  // HATCH JOYMAP
  static const int hatchGrab = 4;
  static const int hatchRelease = 6;
  static const int hatchStow = 12;
  static const int hatchToggleEnabled = 13; // Changes the currently enabled hatch intake


  // BOX INTAKE JOYMAP
  static const int boxIn = 3;
  static const int boxOut = 5;
  static const int boxStow = 11;
};
