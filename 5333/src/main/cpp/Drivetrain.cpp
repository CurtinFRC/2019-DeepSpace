#include "Drivetrain.h"

#include "Robot5333.h"

void curtinfrc::DrivetrainManualStrategy::OnUpdate(double dt) {
  double joyY = 0, joyZ = 0;
  
  if (!_joy.GetRawButton(9)) {
    joyY = -_joy.GetCircularisedAxisAgainst(_joy.kYAxis, _joy.kZAxis) * 0.9;
    joyY *= abs(joyY);
  }

  joyZ = _joy.GetCircularisedAxisAgainst(_joy.kZAxis, _joy.kYAxis) * 0.9;
  // joyZ *= abs(joyZ);

  double leftSpeed = joyY + joyZ;
  double rightSpeed = joyY - joyZ;

  if (_invertedToggle.Update(_joy.GetRawButton(2))) _drivetrain.SetInverted(!_drivetrain.GetInverted());

  _drivetrain.Set(leftSpeed, rightSpeed);

  // curtinfrc::drivetrain has no Update method (yet?)
}

void curtinfrc::DrivetrainFieldOrientedControlStrategy::OnUpdate(double dt) {
  double joyY = -_joy.GetCircularisedAxisAgainst(_joy.kYAxis, _joy.kXAxis);
  double joyZ = _joy.GetCircularisedAxisAgainst(_joy.kXAxis, _joy.kYAxis);

  double mag = sqrt(pow(joyY, 2) + pow(joyZ, 2));
  mag *= std::abs(mag);
  double bearing = atan2(joyZ, joyY);

  if (_invertedToggle.Update(_joy.GetRawButton(2))) _drivetrain.SetInverted(!_drivetrain.GetInverted());

  bearing *= 180 / 3.141592;

  if (mag < 0.05) mag = 0; // CONSTANT; MOVE TO ROBOTMAP

  std::pair<double, double> speed = FOCCalc(mag, bearing, dt, _joy.GetRawButton(9));

  _drivetrain.Set(speed.first, speed.second);

  // curtinfrc::drivetrain has no Update method (yet?)
}

void curtinfrc::DrivetrainPOVSnapStrategy::OnUpdate(double dt) {
  int joyPOV = -_joy.GetPOV();

  if(joyPOV != -1) {
    std::pair<double, double> speed = POVCalc(0.2, joyPOV, dt, _joy.GetRawButton(9));
    _drivetrain.Set(speed.first, speed.second);
  }

  // curtinfrc::drivetrain has no Update method (yet?)
}
