#include "Drivetrain.h"

void curtinfrc::Drivetrain::Set(double leftPower, double rightPower) {
  SetLeft(leftPower);
  SetRight(rightPower);
}

void curtinfrc::Drivetrain::SetVoltage(double left, double right) {
  GetLeft().transmission->SetVoltage(left);
  GetRight().transmission->SetVoltage(right);
}

void curtinfrc::Drivetrain::SetLeft(double leftPower) {
  GetLeft().transmission->SetVoltage(leftPower * 12);
}

void curtinfrc::Drivetrain::SetRight(double rightPower) {
  GetRight().transmission->SetVoltage(rightPower * 12);
}

void curtinfrc::Drivetrain::SetInverted(bool inverted) {
  if (inverted != _config.reversed) {
    _config.reversed = inverted;
    _config.leftDrive.transmission->SetInverted(!_config.leftDrive.transmission->GetInverted());
    _config.rightDrive.transmission->SetInverted(!_config.rightDrive.transmission->GetInverted());
  }
}

double curtinfrc::Drivetrain::GetLeftDistance() {
  auto gb = GetLeft();

  assert(gb.encoder != nullptr);
  return gb.encoder->GetEncoderRotations() * 3.14159265 * 2 * _config.wheelRadius;
}

double curtinfrc::Drivetrain::GetRightDistance() {
  auto gb = GetRight();

  assert(gb.encoder != nullptr);
  return gb.encoder->GetEncoderRotations() * 3.14159265 * 2 * _config.wheelRadius;
}


// Protected

curtinfrc::Gearbox &curtinfrc::Drivetrain::GetLeft() {
  return !_config.reversed ? _config.leftDrive : _config.rightDrive;
}

curtinfrc::Gearbox &curtinfrc::Drivetrain::GetRight() {
  return !_config.reversed ? _config.rightDrive : _config.leftDrive;
}

// DrivetrainFOCController
curtinfrc::DrivetrainFOCController::DrivetrainFOCController(curtinfrc::control::PIDGains gains) : _pid(gains) { }

void curtinfrc::DrivetrainFOCController::SetSetpoint(double magnitude, double bearing, bool hold) {
  _magnitude = magnitude;
  _bearing = bearing;
  _hold = hold;
}

std::pair<double, double> curtinfrc::DrivetrainFOCController::Calculate(double angle, double dt) {
  _pid.SetSetpoint(_bearing);
  _pid.SetWrap(360.0);

  double offset = _pid.Calculate(fmod(angle, 360), dt);

  std::pair<double, double> power{ 0, 0 };
  if (!_hold) {
    power.first = _magnitude * (1 + offset);
    power.second = _magnitude * (1 - offset);
  } else {
    power.first = _magnitude * offset;
    power.second = _magnitude * -offset;
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