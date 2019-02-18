#include "strategies/DriveStrategies.h"

#include <cmath>

#include "ControlMap.h"

using namespace curtinfrc;

BaseDrivetrainTeleopStrategy::BaseDrivetrainTeleopStrategy(std::string name, Drivetrain &drivetrain, ControllerGroup &contGroup)
    : Strategy(name), _drivetrain(drivetrain), _contGroup(contGroup) {
  Requires(&drivetrain);
  SetCanBeInterrupted(true);
  SetCanBeReused(true);
} 

DrivetrainManualStrategy::DrivetrainManualStrategy(Drivetrain &drivetrain, ControllerGroup &contGroup) : BaseDrivetrainTeleopStrategy("Drivetrain Manual", drivetrain, contGroup) { }

void DrivetrainManualStrategy::OnUpdate(double dt) {
  double joyForward = 0, joyTurn = 0;
  
  if (!_contGroup.GetButton(ControlMap::holdMovement)) {
    joyForward = -_contGroup.GetCircularisedAxisAgainst(ControlMap::forwardAxis, ControlMap::turnAxis) * 0.9;
    joyForward *= std::abs(joyForward);
  }

  joyTurn = _contGroup.GetCircularisedAxisAgainst(ControlMap::turnAxis, ControlMap::forwardAxis) * 0.7;
  // joyTurn *= abs(joyTurn);

  double leftSpeed = joyForward + joyTurn;
  double rightSpeed = joyForward - joyTurn;

  if (_invertedToggle.Update(_contGroup.GetButton(ControlMap::reverseDrivetrain)))
    _drivetrain.SetInverted(!_drivetrain.GetInverted());

  _drivetrain.Set(leftSpeed, rightSpeed);
}

DrivetrainFOCStrategy::DrivetrainFOCStrategy(Drivetrain &drivetrain, ControllerGroup &contGroup, control::PIDGains gains) : BaseDrivetrainTeleopStrategy("Drivetrain FOC", drivetrain, contGroup), _foc(gains) { }

void DrivetrainFOCStrategy::OnUpdate(double dt) {
  double joyForward = -_contGroup.GetCircularisedAxisAgainst(ControlMap::forwardAxisFOC, ControlMap::turnAxisFOC);
  double joyTurn = _contGroup.GetCircularisedAxisAgainst(ControlMap::turnAxisFOC, ControlMap::forwardAxisFOC);

  double mag = sqrt(pow(joyForward, 2) + pow(joyTurn, 2));
  mag *= std::abs(mag);
  double bearing = atan2(joyTurn, joyForward);

  if (_invertedToggle.Update(_contGroup.GetButton(ControlMap::reverseDrivetrain))) _drivetrain.SetInverted(!_drivetrain.GetInverted());

  bearing *= 180 / 3.141592;

  if (mag < ControlMap::axisDeadzoneFOC) mag = 0;

  bearing = fmod(bearing + (_drivetrain.GetInverted() ? 180 : 360), 360);
  _foc.SetSetpoint(mag, bearing, _contGroup.GetButton(ControlMap::holdMovement));
  std::pair<double, double> speed = _foc.Calculate(_drivetrain.GetConfig().gyro->GetAngle(), dt);

  _drivetrain.Set(speed.first, speed.second);
}

DrivetrainAngleStrategy::DrivetrainAngleStrategy(Drivetrain &drivetrain, control::PIDGains gains, double angle) : Strategy("Drivetrain Angle"), _drivetrain(drivetrain), _pid(gains), _angle(_drivetrain.GetConfig().gyro->GetAngle()) { }

void DrivetrainAngleStrategy::OnUpdate(double dt) {
  _bearing *= 180 / 3.141592;
  _bearing = fmod(_bearing + (_drivetrain.GetInverted() ? 180 : 360), 360);
  _pid.SetSetpoint(_bearing);
  _pid.SetWrap(360.0);

  double offset = _pid.Calculate(fmod(_angle, 360), dt);
  _drivetrain.Set(offset, -offset);
}
