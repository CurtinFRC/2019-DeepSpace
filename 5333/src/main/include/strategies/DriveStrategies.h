#pragma once

#include "Drivetrain.h"

#include "strategy/Strategy.h"
#include "Elevator.h"
#include "controllers/CurtinControllers.h"
#include "control/PIDController.h"

class BaseDrivetrainTeleopStrategy : public wml::Strategy {
 public:
  BaseDrivetrainTeleopStrategy(std::string name, wml::Drivetrain &drive, wml::controllers::SmartControllerGroup &contGroup);

 protected:
  wml::Drivetrain &_drivetrain;
  wml::controllers::SmartControllerGroup &_contGroup;
};

class DrivetrainManualStrategy : public BaseDrivetrainTeleopStrategy {
 public:
  DrivetrainManualStrategy(wml::Drivetrain &drive, wml::Elevator &elevator, wml::controllers::SmartControllerGroup &contGroup);

  void OnUpdate(double dt) override;

 private:
  wml::Elevator &_elevator;
  wml::Toggle _invertedToggle;
};

class DrivetrainFOCStrategy : public BaseDrivetrainTeleopStrategy {
 public:
  DrivetrainFOCStrategy(wml::Drivetrain &drive, wml::controllers::SmartControllerGroup &contGroup, wml::control::PIDGains gains);

  void OnUpdate(double dt) override;
 private:
  wml::DrivetrainFOCController _foc;
  wml::Toggle _invertedToggle;
};

class DrivetrainAngleStrategy : public wml::Strategy {
 public:
  DrivetrainAngleStrategy(wml::Drivetrain &drivetrain, wml::control::PIDGains gains, double angle);

  void OnUpdate(double dt) override;
 private:
  wml::Drivetrain &_drivetrain;
  wml::control::PIDController _pid;
  double _angle;
};
