#include "Drivetrain.h"

std::string wml::Drivetrain::GetStateString() {
  switch (GetState()) {
   case wml::DrivetrainState::kManual:
    return "kManual";

   case wml::DrivetrainState::kVelocity:
    return "kVelocity";

   case wml::DrivetrainState::kIdle:
    return "kIdle";

   case wml::DrivetrainState::kExternalLoop:
    return "kExternalLoop";
  }

  return "<state error>";
}


void wml::Drivetrain::Set(double leftPower, double rightPower) {
  SetVoltage(leftPower * 12, rightPower * 12);
}

void wml::Drivetrain::SetVoltage(double left, double right) {
  _setpoint = std::pair<double, double>{ left, right };
  SetState(wml::DrivetrainState::kManual);
}

void wml::Drivetrain::SetVelocity(double left, double right) {
  _setpoint = std::pair<double, double>{ left, right };
  SetState(wml::DrivetrainState::kVelocity);
}

void wml::Drivetrain::SetExternalLoop(std::function<std::pair<double, double>(wml::Drivetrain &, double)> func) {
  _externalLoop = func;
  SetState(wml::DrivetrainState::kExternalLoop);
}

void wml::Drivetrain::SetIdle() {
  SetState(wml::DrivetrainState::kIdle);
}

void wml::Drivetrain::SetInverted(bool inverted) {
  if (inverted != _config.reversed) {
    _config.reversed = inverted;
    _config.leftDrive.transmission->SetInverted(!_config.leftDrive.transmission->GetInverted());
    _config.rightDrive.transmission->SetInverted(!_config.rightDrive.transmission->GetInverted());
  }
}

double wml::Drivetrain::GetLeftDistance() {
  auto gb = GetLeft();

  assert(gb.encoder != nullptr);
  return gb.encoder->GetEncoderRotations() * 3.14159265 * 2 * _config.wheelRadius;
}

double wml::Drivetrain::GetRightDistance() {
  auto gb = GetRight();

  assert(gb.encoder != nullptr);
  return gb.encoder->GetEncoderRotations() * 3.14159265 * 2 * _config.wheelRadius;
}


// Protected

void wml::Drivetrain::OnStateChange(DrivetrainState newState, DrivetrainState oldState) {
  switch (newState) {
   case wml::DrivetrainState::kVelocity:
    _pidLeft.SetSetpoint(_setpoint.first);
    _pidRight.SetSetpoint(_setpoint.second);
    break;

   default:
    break;
  }
}

void wml::Drivetrain::OnStatePeriodic(wml::DrivetrainState state, double dt) {
  std::pair<double, double> outputs{ 0, 0 };

  switch (state) {
   case wml::DrivetrainState::kManual:
    outputs = _setpoint;
    break;

   case wml::DrivetrainState::kVelocity:
    _pidLeft.SetSetpoint(_setpoint.first, false);
    _pidRight.SetSetpoint(_setpoint.second, false);

    outputs = {
      _pidLeft.Calculate(GetLeft().encoder->GetEncoderAngularVelocity() * _config.wheelRadius, dt),
      _pidRight.Calculate(GetRight().encoder->GetEncoderAngularVelocity() * _config.wheelRadius, dt)
    };
    break;

   case wml::DrivetrainState::kExternalLoop:
    outputs = _externalLoop(*this, dt);
    break;

   default:
    break;
  }

  GetLeft().transmission->SetVoltage(outputs.first);
  GetRight().transmission->SetVoltage(outputs.second); 
}

wml::Gearbox &wml::Drivetrain::GetLeft() {
  return !_config.reversed ? _config.leftDrive : _config.rightDrive;
}

wml::Gearbox &wml::Drivetrain::GetRight() {
  return !_config.reversed ? _config.rightDrive : _config.leftDrive;
}

// DrivetrainFOCController
wml::DrivetrainFOCController::DrivetrainFOCController(wml::control::PIDGains gains) : _pid(gains) { }

void wml::DrivetrainFOCController::SetSetpoint(double magnitude, double bearing, bool hold) {
  _magnitude = magnitude;
  _bearing = bearing;
  _hold = hold;
}

std::pair<double, double> wml::DrivetrainFOCController::Calculate(double angle, double dt) {
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