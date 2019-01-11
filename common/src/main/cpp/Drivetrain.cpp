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
