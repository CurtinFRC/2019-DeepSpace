#include "Drivetrain.h"

void curtinfrc::DrivetrainManualStrategy::OnUpdate(double dt) {
  double joyY = -_joy.GetCircularisedAxisAgainst(_joy.kYAxis, _joy.kZAxis) * 0.9;
  double joyZ = _joy.GetCircularisedAxisAgainst(_joy.kZAxis, _joy.kYAxis) * 0.65;

  joyY *= abs(joyY);
  joyZ *= abs(joyZ);

  double leftSpeed = joyY + joyZ;
  double rightSpeed = joyY - joyZ;

  if (invertedToggle.Update(_joy.GetRawButton(2))) _drivetrain.SetInverted(!_drivetrain.GetInverted());

  _drivetrain.Set(leftSpeed, rightSpeed);

  // curtinfrc::drivetrain has no Update method (yet?)
}
