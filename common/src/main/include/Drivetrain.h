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
    void SetVoltage(double left, double right);
    void SetLeft(double leftPower);
    void SetRight(double rightPower);

    void SetInverted(bool inverted = false);
    bool GetInverted() { return _config.reversed; };

    DrivetrainConfig &GetConfig() { return _config; };

    double GetLeftDistance();
    double GetRightDistance();

   protected:
    Gearbox &GetLeft();
    Gearbox &GetRight();

   private:
    DrivetrainConfig _config;

    Usage<DrivetrainConfig>::Scoped _usage{&_config};
  };

  class DrivetrainFOCController {
   public:
    DrivetrainFOCController(control::PIDGains gains);

    void SetSetpoint(double magnitude, double bearing, bool hold = false);
    std::pair<double, double> Calculate(double angle, double dt);
   
   private:
    control::PIDController _pid;
    double _magnitude = 0;
    double _bearing = 0;
    bool _hold = false;
  };
} // ns curtinfrc
