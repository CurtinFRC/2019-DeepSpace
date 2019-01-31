#pragma once

#include <frc/SpeedController.h>

#include "Gearbox.h"

#include "Usage.h"
#include "CurtinControllers.h"

namespace curtinfrc {
  struct DrivetrainConfig {
    Gearbox &leftDrive;
    Gearbox &rightDrive;

    /**
     * The emprical trackwidth between the left and right wheels, in metres
     */
    double trackwidth;

    /**
     * The trackdepth, from front of robot to back of robot, in metres.
     */
    double trackdepth;

    /**
     * The wheel radius, in metres
     */
    double wheelRadius;

    /**
     * The mass of the robot chassis, in kg
     */
    double mass;
  };
  
  class Drivetrain {
   public:
    Drivetrain(DrivetrainConfig config) : _config(config) {};

    void Set(double leftPower, double rightPower);
    void SetLeft(double leftPower);
    void SetRight(double rightPower);

   private:
    DrivetrainConfig _config;

    Usage<DrivetrainConfig>::Scoped _usage{&_config};
  };

  class DrivetrainController {
   public:
    DrivetrainController(Drivetrain &drivetrain, curtinfrc::Joystick &joy) : _drivetrain(drivetrain), _joy(joy) {};
    void Update(double dt);

   private:
    Drivetrain &_drivetrain;
    curtinfrc::Joystick &_joy;
  };
} // ns curtinfrc
