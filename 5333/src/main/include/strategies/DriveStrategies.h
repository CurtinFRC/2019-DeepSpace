#pragma once

#include "Drivetrain.h"
#include "strategy/Strategy.h"

class BaseDrivetrainTeleopStrategy : public curtinfrc::Strategy {
 public:
  BaseDrivetrainTeleopStrategy(std::string name, curtinfrc::Drivetrain &drive, curtinfrc::JoystickGroup &joys);

 protected:
  curtinfrc::Drivetrain &_drivetrain;
  curtinfrc::JoystickGroup &_joys;
};

class DrivetrainManualStrategy : public BaseDrivetrainTeleopStrategy {
 public:
  DrivetrainManualStrategy(curtinfrc::Drivetrain &drive, curtinfrc::JoystickGroup &joys);

  void OnUpdate(double dt) override;
 private:
  curtinfrc::Toggle _invertedToggle;
};

class DrivetrainFOCStrategy : public BaseDrivetrainTeleopStrategy {
 public:
  DrivetrainFOCStrategy(curtinfrc::Drivetrain &drive, curtinfrc::JoystickGroup &joys, curtinfrc::control::PIDGains gains);

  void OnUpdate(double dt) override;
 private:
  curtinfrc::DrivetrainFOCController _foc;
  curtinfrc::Toggle _invertedToggle;
};

class DrivetrainAngleStrategy : public Strategy {
 public:
  DrivetrainAngleStrategy(Drivetrain &drivetrain, control::PIDGains gains, double angle);

  void OnUpdate(double dt) override;
 private:
  Drivetrain &_drivetrain;
  control::PIDController _pid;
  double _angle;
  double _bearing = 0;
};
