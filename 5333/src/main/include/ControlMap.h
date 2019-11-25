#pragma once

#include <vector>

#include "controllers/CurtinControllers.h"

#define N_CONT 3


class ControlMap {
 public:
  static void InitSmartControllerGroup(wml::controllers::SmartControllerGroup &contGroup);

  // DRIVETRAIN AXIS JOYMAP
  static const wml::controllers::tAxis forwardAxis;
  static const wml::controllers::tAxis turnAxis;
  static const wml::controllers::tAxis forwardAxisFOC;
  static const wml::controllers::tAxis turnAxisFOC;

  static const double axisDeadzone;
  static const double axisDeadzoneFOC;


  // DRIVETRAIN JOYMAP
  static const std::vector<wml::controllers::tButton> reverseDrivetrain;   // Toggles the direction of the drivetrain
  static const std::vector<wml::controllers::tButton> holdMovement;        // Makes the robot 'line up' (0 magnitude but still rotating)
  static const std::vector<wml::controllers::tButton> activateFOC;        // Toggles the drivetrain between Manual and FOC control
  static const wml::controllers::tAxis                drivetrainThrottle;
  static const std::vector<wml::controllers::tButton> chargeForward;

  static const double drivetrainMinThrottle;
  static const double drivetrainForwardThrottle;
  static const double drivetrainTurnThrottle;


  // BEELEVATOR JOYMAP
  static const std::vector<wml::controllers::tButton> raiseLift;
  static const std::vector<wml::controllers::tButton> lowerLift;

  static const std::vector<wml::controllers::tButton> liftGoalGround;
  static const std::vector<wml::controllers::tButton> liftGoalLower1;
  static const std::vector<wml::controllers::tButton> liftGoalLower2;
  static const std::vector<wml::controllers::tButton> liftGoalMiddle1;
  static const std::vector<wml::controllers::tButton> liftGoalMiddle2;
  static const std::vector<wml::controllers::tButton> liftGoalUpper1;
  static const std::vector<wml::controllers::tButton> liftGoalUpper2;

  static const double liftSetpointGround; // Exact values need to be tested *
  static const double liftSetpointLower1;
  static const double liftSetpointLower2;
  static const double liftSetpointMiddle1;
  static const double liftSetpointMiddle2;
  static const double liftSetpointUpper1;
  static const double liftSetpointUpper2;


  // HATCH JOYMAP
  static const std::vector<wml::controllers::tButton> hatchGrab;
  static const std::vector<wml::controllers::tButton> hatchRelease;
  static const std::vector<wml::controllers::tButton> hatchStow;
  static const std::vector<wml::controllers::tButton> hatchToggleEnabled; // Changes the currently enabled hatch intake


  // BOX INTAKE JOYMAP
  static const std::vector<wml::controllers::tButton> boxIn;
  static const std::vector<wml::controllers::tButton> boxOut;
  static const std::vector<wml::controllers::tButton> boxStow;

  static const double boxDeployedThrottle;
  static const double boxStowingThrottle;
  static const double boxStowedThrottle;


  // CONTROLSYSTEM JOYMAP
  static const std::vector<wml::controllers::tButton> compressorOn;
};
