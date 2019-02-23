#pragma once

#include "CurtinControllers.h"

#define N_CONT 3

class ControlMap {
 public:
  // DRIVETRAIN AXIS JOYMAP
  static const curtinfrc::tControllerAxis forwardAxis;
  static const curtinfrc::tControllerAxis turnAxis;
  static const curtinfrc::tControllerAxis forwardAxisFOC;
  static const curtinfrc::tControllerAxis turnAxisFOC;

  static const double axisDeadzone;
  static const double axisDeadzoneFOC;


  // DRIVETRAIN JOYMAP
  static const curtinfrc::tControllerButtonMap reverseDrivetrain;   // Toggles the direction of the drivetrain
  static const curtinfrc::tControllerButtonMap holdMovement;        // Makes the robot 'line up' (0 magnitude but still rotating)
  static const curtinfrc::tControllerButtonMap activateFOC;        // Toggles the drivetrain between Manual and FOC control


  // BEELEVATOR JOYMAP
  static const curtinfrc::tControllerButtonMap raiseLift;
  static const curtinfrc::tControllerButtonMap lowerLift;

  #if N_CONT == 2
  static const curtinfrc::tControllerButtonMap liftGoalGround;
  static const curtinfrc::tControllerButtonMap liftGoalLower1;
  static const curtinfrc::tControllerButtonMap liftGoalLower2;
  static const curtinfrc::tControllerButtonMap liftGoalMiddle1;
  static const curtinfrc::tControllerButtonMap liftGoalMiddle2;
  static const curtinfrc::tControllerButtonMap liftGoalUpper1;
  static const curtinfrc::tControllerButtonMap liftGoalUpper2;
  #elif N_CONT == 3
  static const int liftSelectorID = 0;
  static const curtinfrc::tControllerSelectorMapping liftSelectorConfig;

  static const curtinfrc::tControllerSelectorButtonMap liftGoalGround;
  static const curtinfrc::tControllerSelectorButtonMap liftGoalLower1;
  static const curtinfrc::tControllerSelectorButtonMap liftGoalLower2;
  static const curtinfrc::tControllerSelectorButtonMap liftGoalMiddle1;
  static const curtinfrc::tControllerSelectorButtonMap liftGoalMiddle2;
  static const curtinfrc::tControllerSelectorButtonMap liftGoalUpper1;
  static const curtinfrc::tControllerSelectorButtonMap liftGoalUpper2;
  #endif

  static const double liftSetpointGround; // Exact values need to be tested *
  static const double liftSetpointLower1;
  static const double liftSetpointLower2;
  static const double liftSetpointMiddle1;
  static const double liftSetpointMiddle2;
  static const double liftSetpointUpper1;
  static const double liftSetpointUpper2;


  // HATCH JOYMAP
  static const curtinfrc::tControllerButtonMap hatchGrab;
  static const curtinfrc::tControllerButtonMap hatchRelease;
  static const curtinfrc::tControllerButtonMap hatchStow;
  static const curtinfrc::tControllerButtonMap hatchToggleEnabled; // Changes the currently enabled hatch intake


  // BOX INTAKE JOYMAP
  static const curtinfrc::tControllerButtonMap boxIn;
  static const curtinfrc::tControllerButtonMap boxOut;
  static const curtinfrc::tControllerButtonMap boxStow;

  static const double boxDeployedThrottle;
  static const double boxStowingThrottle;
  static const double boxStowedThrottle;


  // CONTROLSYSTEM JOYMAP
  static const curtinfrc::tControllerButtonMap compressorOn;
};
