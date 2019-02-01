#include "Drivetrain.h"

void curtinfrc::Drivetrain::SetLeft(double leftPower) {
  _config.leftDrive.transmission->Set(leftPower);
}

void curtinfrc::Drivetrain::SetRight(double rightPower) {
  _config.rightDrive.transmission->Set(rightPower);
}

void curtinfrc::Drivetrain::Set(double leftPower, double rightPower) {
  SetLeft(leftPower);
  SetRight(rightPower);
}


void curtinfrc::DrivetrainController::Update(double dt) {
  double joyY = -_joy.GetCircularisedAxisAgainst(_joy.kYAxis, _joy.kZAxis) * 0.9;
  double joyZ = _joy.GetCircularisedAxisAgainst(_joy.kZAxis, _joy.kYAxis) * 0.65;

  joyY *= abs(joyY);
  joyZ *= abs(joyZ);

  double leftSpeed = joyY + joyZ;
  double rightSpeed = joyY - joyZ;

  _drivetrain.Set(leftSpeed, rightSpeed);
}
