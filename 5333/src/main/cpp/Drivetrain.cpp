#include "Drivetrain.h"
#include "ControlMap.h"

void curtinfrc::DrivetrainManualStrategy::OnUpdate(double dt) {
  double joyForward = 0, joyTurn = 0;
  
  if (!_joyGroup.GetButton(ControlMap::holdMovement)) {
    joyForward = -_joyGroup.GetJoystick((JoystickGroup::JoyNum)1).GetCircularisedAxisAgainst(ControlMap::forwardAxis, ControlMap::turnAxis) * 0.9;
    joyForward *= abs(joyForward);
  }

  joyTurn = _joyGroup.GetJoystick((JoystickGroup::JoyNum)1).GetCircularisedAxisAgainst(ControlMap::turnAxis, ControlMap::forwardAxis) * 0.9;
  // joyTurn *= abs(joyTurn);

  double leftSpeed = joyForward + joyTurn;
  double rightSpeed = joyForward - joyTurn;

  if (_invertedToggle.Update(_joyGroup.GetButton(ControlMap::reverseDrivetrain))) _drivetrain.SetInverted(!_drivetrain.GetInverted());

  _drivetrain.Set(leftSpeed, rightSpeed);

  // curtinfrc::drivetrain has no Update method (yet?)
}

void curtinfrc::DrivetrainFieldOrientedControlStrategy::OnUpdate(double dt) {
  double joyForward = -_joyGroup.GetJoystick((JoystickGroup::JoyNum)1).GetCircularisedAxisAgainst(ControlMap::forwardAxisFOC, ControlMap::turnAxisFOC);
  double joyTurn = _joyGroup.GetJoystick((JoystickGroup::JoyNum)1).GetCircularisedAxisAgainst(ControlMap::turnAxisFOC, ControlMap::forwardAxisFOC);

  double mag = sqrt(pow(joyForward, 2) + pow(joyTurn, 2));
  mag *= std::abs(mag);
  double bearing = atan2(joyTurn, joyForward);

  if (_invertedToggle.Update(_joyGroup.GetButton(ControlMap::reverseDrivetrain))) _drivetrain.SetInverted(!_drivetrain.GetInverted());

  bearing *= 180 / 3.141592;

  if (mag < ControlMap::axisDeadzoneFOC) mag = 0;

  std::pair<double, double> speed = FOCCalc(mag, bearing, dt, _joyGroup.GetButton(ControlMap::holdMovement));

  _drivetrain.Set(speed.first, speed.second);

  // curtinfrc::drivetrain has no Update method (yet?)
}
