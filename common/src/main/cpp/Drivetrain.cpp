#include "Drivetrain.h"

std::string curtinfrc::RawDrivetrain::GetState() {
  switch (_drivetrain->GetState()) {
   case curtinfrc::DrivetrainState::kManual:
    return "kManual";

   case curtinfrc::DrivetrainState::kVelocity:
    return "kVelocity";

   case curtinfrc::DrivetrainState::kIdle:
    return "kIdle";

   case curtinfrc::DrivetrainState::kExternalLoop:
    return "kExternalLoop";
  }

  return "<state error>";
}


curtinfrc::devices::RawStateDevice *curtinfrc::Drivetrain::MakeRawStateDevice(std::string name) {
  return new RawDrivetrain(this, name);
}

void curtinfrc::Drivetrain::Set(double leftPower, double rightPower) {
  SetVoltage(leftPower * 12, rightPower * 12);
}

void curtinfrc::Drivetrain::SetVoltage(double left, double right) {
  _setpoint = std::pair<double, double>{ left, right };
  SetState(curtinfrc::DrivetrainState::kManual);
}

void curtinfrc::Drivetrain::SetVelocity(double left, double right) {
  _setpoint = std::pair<double, double>{ left, right };
  SetState(curtinfrc::DrivetrainState::kVelocity);
}

void curtinfrc::Drivetrain::SetExternalLoop(std::function<std::pair<double, double>(curtinfrc::Drivetrain &, double)> func) {
  _externalLoop = func;
  SetState(curtinfrc::DrivetrainState::kExternalLoop);
}

void curtinfrc::Drivetrain::SetIdle() {
  SetState(curtinfrc::DrivetrainState::kIdle);
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

void curtinfrc::Drivetrain::OnStateChange(DrivetrainState newState, DrivetrainState oldState) {
  switch (newState) {
   case curtinfrc::DrivetrainState::kVelocity:
    _pidLeft.SetSetpoint(_setpoint.first);
    _pidRight.SetSetpoint(_setpoint.second);
    break;

   default:
    break;
  }
}

void curtinfrc::Drivetrain::OnStatePeriodic(curtinfrc::DrivetrainState state, double dt) {
  std::pair<double, double> outputs{ 0, 0 };

  switch (state) {
   case curtinfrc::DrivetrainState::kManual:
    outputs = _setpoint;
    break;

   case curtinfrc::DrivetrainState::kVelocity:
    _pidLeft.SetSetpoint(_setpoint.first, false);
    _pidRight.SetSetpoint(_setpoint.second, false);

    outputs = {
      _pidLeft.Calculate(GetLeft().encoder->GetEncoderAngularVelocity() * _config.wheelRadius, dt),
      _pidRight.Calculate(GetRight().encoder->GetEncoderAngularVelocity() * _config.wheelRadius, dt)
    };
    break;

   case curtinfrc::DrivetrainState::kExternalLoop:
    outputs = _externalLoop(*this, dt);
    break;

   default:
    break;
  }

  GetLeft().transmission->SetVoltage(outputs.first);
  GetRight().transmission->SetVoltage(outputs.second); 
}

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