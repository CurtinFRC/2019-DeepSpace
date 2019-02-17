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

  static const curtinfrc::tJoymap liftGoalGround;
  static const curtinfrc::tJoymap liftGoalLower1;
  static const curtinfrc::tJoymap liftGoalLower2;
  static const curtinfrc::tJoymap liftGoalMiddle1;
  static const curtinfrc::tJoymap liftGoalMiddle2;
  static const curtinfrc::tJoymap liftGoalUpper1;
  static const curtinfrc::tJoymap liftGoalUpper2;

  static const double liftSetpointGround; // Exact values need to be tested *
  static const double liftSetpointLower1;
  static const double liftSetpointLower2;
  static const double liftSetpointMiddle1;
  static const double liftSetpointMiddle2;
  static const double liftSetpointUpper1;
  static const double liftSetpointUpper2;


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

  static const double boxDeployedThrottle;
  static const double boxStowingThrottle;
  static const double boxStowedThrottle;


  // CONTROLSYSTEM JOYMAP
  static const curtinfrc::tJoymap compressorOn;
};
