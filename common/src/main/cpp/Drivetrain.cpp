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


std::pair<double, double> curtinfrc::DrivetrainFieldOrientedControlStrategy::FOCCalc(double mag, double bearing, double dt, bool hold) {
  bearing = fmod(bearing + (_drivetrain.GetInverted() ? 180 : 360), 360);

  _controller.SetSetpoint(bearing);
  _controller.SetWrap(360.0);

  double offset = _controller.Calculate(fmod(_drivetrain.GetConfig().gyro->GetAngle(), 360), dt);

  std::pair<double, double> power{ 0, 0 };
  if (!hold) {
    power.first = mag * (1 + offset);
    power.second = mag * (1 - offset);
  } else {
    power.first = mag * offset;
    power.second = mag * -offset;
  }

  // Prevent the robot from losing control by going >1 (or <-1) on left and right gearboxes
  double aFirst = std::abs(power.first);
  double aSecond = std::abs(power.second);

  if (aFirst > aSecond) {
    if (aFirst > 1) { // MAXIMUM EFFICIENCY... (I mean I probably need to speed it up wherever I can given my general coding practices...)
      power.second /= aFirst;
      power.first /= aFirst;
    }
  } else if (aSecond > 1) {
    power.first /= aSecond;
    power.second /= aSecond;
  }

  return power;
}
