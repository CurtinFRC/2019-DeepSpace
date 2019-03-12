#include "strategies/DriveStrategies.h"

#include <cmath>

#include "ControlMap.h"

using namespace curtinfrc;
using namespace curtinfrc::controllers;

BaseDrivetrainTeleopStrategy::BaseDrivetrainTeleopStrategy(std::string name, Drivetrain &drivetrain, SmartControllerGroup &contGroup)
    : Strategy(name), _drivetrain(drivetrain), _contGroup(contGroup) {
  Requires(&drivetrain);
  SetCanBeInterrupted(true);
  SetCanBeReused(true);
} 

DrivetrainManualStrategy::DrivetrainManualStrategy(Drivetrain &drivetrain, SmartControllerGroup &contGroup) : BaseDrivetrainTeleopStrategy("Drivetrain Manual", drivetrain, contGroup) { }

void DrivetrainManualStrategy::OnUpdate(double dt) {
  double joyForward = 0, joyTurn = 0;
  
  if (!_contGroup.Get(ControlMap::holdMovement)) {
    joyForward = -_contGroup.Get(ControlMap::forwardAxis) * 0.9;
    joyForward *= std::abs(joyForward);
  }

  joyTurn = _contGroup.Get(ControlMap::turnAxis) * 0.7;
  // joyTurn *= abs(joyTurn);

  double leftSpeed = joyForward + joyTurn;
  double rightSpeed = joyForward - joyTurn;

  if (_invertedToggle.Update(_contGroup.Get(ControlMap::reverseDrivetrain)))
    _drivetrain.SetInverted(!_drivetrain.GetInverted());

  _drivetrain.Set(leftSpeed, rightSpeed);
}

DrivetrainFOCStrategy::DrivetrainFOCStrategy(Drivetrain &drivetrain, SmartControllerGroup &contGroup, control::PIDGains gains) : BaseDrivetrainTeleopStrategy("Drivetrain FOC", drivetrain, contGroup), _foc(gains) { }

void DrivetrainFOCStrategy::OnUpdate(double dt) {
  double joyForward = -_contGroup.Get(ControlMap::forwardAxisFOC);
  double joyTurn = _contGroup.Get(ControlMap::turnAxisFOC);

  double mag = sqrt(pow(joyForward, 2) + pow(joyTurn, 2));
  mag *= std::abs(mag);
  double bearing = atan2(joyTurn, joyForward);

  if (_invertedToggle.Update(_contGroup.Get(ControlMap::reverseDrivetrain))) _drivetrain.SetInverted(!_drivetrain.GetInverted());

  bearing *= 180 / 3.141592;

  if (mag < ControlMap::axisDeadzoneFOC) mag = 0;

  bearing = fmod(bearing + (_drivetrain.GetInverted() ? 180 : 360), 360);
  _foc.SetSetpoint(mag, bearing, _contGroup.Get(ControlMap::holdMovement));
  std::pair<double, double> speed = _foc.Calculate(_drivetrain.GetConfig().gyro->GetAngle(), dt);

  _drivetrain.Set(speed.first, speed.second);
}

DrivetrainAngleStrategy::DrivetrainAngleStrategy(Drivetrain &drivetrain, control::PIDGains gains, double angle) : Strategy("Drivetrain Angle"), _drivetrain(drivetrain), _pid(gains) {
  Requires(&drivetrain);
  SetCanBeInterrupted(true);
  SetCanBeReused(false);

  _angle = fmod(angle + (_drivetrain.GetInverted() ? 180 : 360), 360);
  _pid.SetSetpoint(_angle);
  _pid.SetWrap(360.0);

  _drivetrain.SetExternalLoop([&](Drivetrain &, double dt) {
    double offset = _pid.Calculate(_drivetrain.GetConfig().gyro->GetAngle(), dt);
    return std::pair<double, double>(offset, -offset);
  });
}

void DrivetrainAngleStrategy::OnUpdate(double dt) {
  if (_pid.IsDone())
    SetDone();
}
