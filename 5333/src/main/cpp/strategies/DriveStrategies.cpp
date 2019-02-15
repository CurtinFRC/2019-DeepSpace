#include "strategies/DriveStrategies.h"

#include "ControlMap.h"

using namespace curtinfrc;

BaseDrivetrainTeleopStrategy::BaseDrivetrainTeleopStrategy(std::string name, Drivetrain &drivetrain,
                                                           JoystickGroup &joys)
    : Strategy(name), _drivetrain(drivetrain), _joys(joys) {
  Requires(&drivetrain);
  SetCanBeInterrupted(true);
  SetCanBeReused(true);
} 

DrivetrainManualStrategy::DrivetrainManualStrategy(Drivetrain &drivetrain, JoystickGroup &joys) : BaseDrivetrainTeleopStrategy("Drivetrain Manual", drivetrain, joys) { }

void DrivetrainManualStrategy::OnUpdate(double dt) {
  double joyForward = 0, joyTurn = 0;
  
  if (!_joys.GetButton(ControlMap::holdMovement)) {
    joyForward = -_joys.GetJoystick((JoystickGroup::JoyNum)1).GetCircularisedAxisAgainst(ControlMap::forwardAxis, ControlMap::turnAxis) * 0.9;
    joyForward *= abs(joyForward);
  }

  joyTurn = _joys.GetJoystick((JoystickGroup::JoyNum)1).GetCircularisedAxisAgainst(ControlMap::turnAxis, ControlMap::forwardAxis) * 0.9;
  // joyTurn *= abs(joyTurn);

  double leftSpeed = joyForward + joyTurn;
  double rightSpeed = joyForward - joyTurn;

  if (_invertedToggle.Update(_joys.GetButton(ControlMap::reverseDrivetrain)))
    _drivetrain.SetInverted(!_drivetrain.GetInverted());

  _drivetrain.Set(leftSpeed, rightSpeed);
}

DrivetrainFOCStrategy::DrivetrainFOCStrategy(Drivetrain &drivetrain, JoystickGroup &joys, control::PIDGains gains) : BaseDrivetrainTeleopStrategy("Drivetrain Manual", drivetrain, joys), _foc(gains) { }

void DrivetrainFOCStrategy::OnUpdate(double dt) {
  double joyForward = -_joys.GetJoystick((JoystickGroup::JoyNum)1).GetCircularisedAxisAgainst(ControlMap::forwardAxisFOC, ControlMap::turnAxisFOC);
  double joyTurn = _joys.GetJoystick((JoystickGroup::JoyNum)1).GetCircularisedAxisAgainst(ControlMap::turnAxisFOC, ControlMap::forwardAxisFOC);

  double mag = sqrt(pow(joyForward, 2) + pow(joyTurn, 2));
  mag *= std::abs(mag);
  double bearing = atan2(joyTurn, joyForward);

  if (_invertedToggle.Update(_joys.GetButton(ControlMap::reverseDrivetrain))) _drivetrain.SetInverted(!_drivetrain.GetInverted());

  bearing *= 180 / 3.141592;

  if (mag < ControlMap::axisDeadzoneFOC) mag = 0;

  bearing = fmod(bearing + (_drivetrain.GetInverted() ? 180 : 360), 360);
  _foc.SetSetpoint(mag, bearing, _joys.GetButton(ControlMap::holdMovement));
  std::pair<double, double> speed = _foc.Calculate(_drivetrain.GetConfig().gyro->GetAngle(), dt);

  _drivetrain.Set(speed.first, speed.second);
}