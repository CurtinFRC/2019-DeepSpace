#pragma once

#include <utility>
#include <cmath>
#include <functional>

#include <frc/SpeedController.h>
#include <frc/interfaces/Gyro.h>
#include <frc/GenericHID.h>

#include "Gearbox.h"
#include "strategy/Strategy.h"
#include "strategy/StrategySystem.h"
#include "control/PIDController.h"
#include "devices/StateDevice.h"

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
  
  enum class DrivetrainState { kManual = 0, kVelocity, kIdle, kExternalLoop };

  class Drivetrain : public devices::StateDevice<DrivetrainState>, public StrategySystem {
   public:
    Drivetrain(DrivetrainConfig config, control::PIDGains gains = { "Drivetrain Velocity" }) : _config(config), _pidLeft(gains), _pidRight(gains) {};

    void Set(double leftPower, double rightPower);
    void SetVoltage(double left, double right);
    void SetVelocity(double left, double right);
    void SetExternalLoop(std::function<std::pair<double, double>(Drivetrain &, double)> func);
    void SetIdle();

    void SetInverted(bool inverted = false);
    bool GetInverted() { return _config.reversed; };

    DrivetrainConfig &GetConfig() { return _config; };

    double GetLeftDistance();
    double GetRightDistance();

   protected:
    void OnStateChange(DrivetrainState newState, DrivetrainState oldState) override;
    void OnStatePeriodic(DrivetrainState state, double dt) override;

    Gearbox &GetLeft();
    Gearbox &GetRight();

   private:
    control::PIDController _pidLeft, _pidRight;
    std::pair<double, double> _setpoint;
    std::function<std::pair<double,double>(Drivetrain &,double)> _externalLoop;

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
