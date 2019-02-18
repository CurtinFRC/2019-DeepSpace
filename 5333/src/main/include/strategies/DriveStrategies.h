#pragma once

#include "Drivetrain.h"
#include "strategy/Strategy.h"
#include "control/PIDController.h"

class BaseDrivetrainTeleopStrategy : public curtinfrc::Strategy {
 public:
  BaseDrivetrainTeleopStrategy(std::string name, curtinfrc::Drivetrain &drive, curtinfrc::ControllerGroup &contGroup);

 protected:
  curtinfrc::Drivetrain &_drivetrain;
  curtinfrc::ControllerGroup &_contGroup;
};

class DrivetrainManualStrategy : public BaseDrivetrainTeleopStrategy {
 public:
  DrivetrainManualStrategy(curtinfrc::Drivetrain &drive, curtinfrc::ControllerGroup &contGroup);

  void OnUpdate(double dt) override;
 private:
  curtinfrc::Toggle _invertedToggle;
};

class DrivetrainFOCStrategy : public BaseDrivetrainTeleopStrategy {
 public:
  DrivetrainFOCStrategy(curtinfrc::Drivetrain &drive, curtinfrc::ControllerGroup &contGroup, curtinfrc::control::PIDGains gains);

  void OnUpdate(double dt) override;
 private:
  curtinfrc::DrivetrainFOCController _foc;
  curtinfrc::Toggle _invertedToggle;
};

class DrivetrainAngleStrategy : public curtinfrc::Strategy {
 public:
  DrivetrainAngleStrategy(curtinfrc::Drivetrain &drivetrain, curtinfrc::control::PIDGains gains, double angle);

  void OnUpdate(double dt) override;
 private:
  curtinfrc::Drivetrain &_drivetrain;
  curtinfrc::control::PIDController _pid;
  double _angle;
  double _bearing = 0;
};
