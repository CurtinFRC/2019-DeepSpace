#pragma once

#include <frc/SpeedController.h>
#include <frc/interfaces/Gyro.h>

#include "Gearbox.h"
#include "strategy/Strategy.h"
#include "strategy/StrategySystem.h"

#include "Usage.h"
#include "CurtinControllers.h"
#include "Toggle.h"

namespace curtinfrc {
  struct DrivetrainConfig {
    Gearbox &leftDrive;
    Gearbox &rightDrive;

    /**
     * The gyroscope for the drivetrain, for closed-loop driving. May be
     * null.
     */
    frc::Gyro *gyro;

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

    bool reversed = false;
  };
  
  class Drivetrain : public StrategySystem {
   public:
    Drivetrain(DrivetrainConfig config) : _config(config) {};

    void Set(double leftPower, double rightPower);
    void SetLeft(double leftPower);
    void SetRight(double rightPower);

    void SetInverted(bool inverted = false);
    bool GetInverted() { return _config.reversed; };

   protected:
    Gearbox &GetLeft();
    Gearbox &GetRight();

   private:
    DrivetrainConfig _config;

    Usage<DrivetrainConfig>::Scoped _usage{&_config};
  };

  class DrivetrainManualStrategy : public Strategy {
   public:
    DrivetrainManualStrategy(Drivetrain &drivetrain, curtinfrc::Joystick &joy) : Strategy("Drivetrain Manual"), _drivetrain(drivetrain), _joy(joy) {
      Requires(&drivetrain);
      SetCanBeInterrupted(true);
      SetCanBeReused(true);
    };
    
    void OnUpdate(double dt) override; // Should be defined in a team specific file

   private:
    Drivetrain &_drivetrain;
    curtinfrc::Joystick &_joy;

    Toggle invertedToggle;
  };
} // ns curtinfrc
