#pragma once

#include <utility>
#include <cmath>

#include <frc/SpeedController.h>
#include <frc/interfaces/Gyro.h>
#include <frc/GenericHID.h>

#include "Gearbox.h"
#include "strategy/Strategy.h"
#include "strategy/StrategySystem.h"
#include "control/PIDController.h"

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

    DrivetrainConfig &GetConfig() { return _config; };

   protected:
    Gearbox &GetLeft();
    Gearbox &GetRight();

   private:
    DrivetrainConfig _config;

    Usage<DrivetrainConfig>::Scoped _usage{&_config};
  };

  class DrivetrainManualStrategy : public Strategy {
   public:
    DrivetrainManualStrategy(Drivetrain &drivetrain, curtinfrc::JoystickGroup &joyGroup) : Strategy("Drivetrain Manual"), _drivetrain(drivetrain), _joyGroup(joyGroup) {
      Requires(&drivetrain);
      SetCanBeInterrupted(true);
      SetCanBeReused(true);
    };
    
    virtual void OnUpdate(double dt) override; // Should be defined in a team specific file

   protected:
    Drivetrain &_drivetrain;
    curtinfrc::JoystickGroup &_joyGroup;

    Toggle _invertedToggle;
  };

  class DrivetrainFieldOrientedControlStrategy : public Strategy {
   public:
    DrivetrainFieldOrientedControlStrategy(Drivetrain &drivetrain, curtinfrc::JoystickGroup &joyGroup, control::PIDGains gains) : Strategy("Drivetrain Field Oriented Control"), _drivetrain(drivetrain), _joyGroup(joyGroup), _controller(gains) {
      Requires(&drivetrain);
      SetCanBeInterrupted(true);
      SetCanBeReused(true);
    };

    virtual void OnUpdate(double dt) override; // Should be defined in a team specific file (calc with FOCCalc())
    std::pair<double, double> FOCCalc(double mag, double bearing, double dt, bool hold = false); // bearing in degrees

   protected:
    Drivetrain &_drivetrain;
    curtinfrc::JoystickGroup &_joyGroup;

    control::PIDController _controller;
    Toggle _invertedToggle;
  };

  class DrivetrainPOVSnapStrategy : public Strategy {
   public:
    DrivetrainPOVSnapStrategy(Drivetrain &drivetrain, curtinfrc::Joystick &joy, control::PIDGains gains) : Strategy("Drivetrain Field Oriented Control"), _drivetrain(drivetrain), _joy(joy), _controller(gains) {
      Requires(&drivetrain);
      SetCanBeInterrupted(true);
      SetCanBeReused(true);
    };

    virtual void OnUpdate(double dt) override; // Should be defined in a team specific file (calc with FOCCalc())
    std::pair<double, double> POVCalc(double mag, double bearing, double dt, bool hold = false); // bearing in degrees

   protected:
    Drivetrain &_drivetrain;
    curtinfrc::Joystick &_joy;

    control::PIDController _controller;
    Toggle _invertedToggle;
  };
} // ns curtinfrc
