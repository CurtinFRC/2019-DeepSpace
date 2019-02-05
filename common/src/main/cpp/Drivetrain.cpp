#include "Drivetrain.h"


void curtinfrc::Drivetrain::Set(double leftPower, double rightPower) {
  SetLeft(leftPower);
  SetRight(rightPower);
}

void curtinfrc::Drivetrain::SetLeft(double leftPower) {
  GetLeft().transmission->Set(leftPower);
}

void curtinfrc::Drivetrain::SetRight(double rightPower) {
  GetRight().transmission->Set(rightPower);
}

void curtinfrc::Drivetrain::SetInverted(bool inverted) {
  if (inverted != _config.reversed) {
    _config.reversed = inverted;
    _config.leftDrive.transmission->SetInverted(!_config.leftDrive.transmission->GetInverted());
    _config.rightDrive.transmission->SetInverted(!_config.rightDrive.transmission->GetInverted());
  }
}


// Protected

curtinfrc::Gearbox &curtinfrc::Drivetrain::GetLeft() {
  return !_config.reversed ? _config.leftDrive : _config.rightDrive;
}

curtinfrc::Gearbox &curtinfrc::Drivetrain::GetRight() {
  return !_config.reversed ? _config.rightDrive : _config.leftDrive;
}
