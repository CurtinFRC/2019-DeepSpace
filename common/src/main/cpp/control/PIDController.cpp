#include "control/PIDController.h"

#include <cmath>
#include <frc/smartdashboard/SendableBase.h>
#include <frc/smartdashboard/SendableBuilder.h>

#include <networktables/NetworkTableInstance.h>

#include <iostream>

using namespace curtinfrc::control;

// PIDGains

PIDGains::PIDGains(std::string name, double kP, double kI, double kD, double kF) : _name(name), _kP(kP), _kI(kI), _kD(kD), _kF(kF) {
  _table = nt::NetworkTableInstance::GetDefault().GetTable("PIDGains[" + name + "]");

  _ntbounds.emplace_back(_table, "kP", &_kP);
  _ntbounds.emplace_back(_table, "kI", &_kI);
  _ntbounds.emplace_back(_table, "kD", &_kD);
  _ntbounds.emplace_back(_table, "kF", &_kF);
}

double PIDGains::kP() const {
  return _kP;
}

double PIDGains::kI() const {
  return _kI;
}

double PIDGains::kD() const {
  return _kD;
}

double PIDGains::kF() const {
  return _kF;
}

// PIDController

PIDController::PIDController(PIDGains gains) : _gains(gains), _lastError(0) {}

void PIDController::SetSetpoint(double setpoint) {
  _setpoint = setpoint;
}

double PIDController::GetSetpoint() {
  return _setpoint;
}

double PIDController::Calculate(double processVariable, double dt) {
  double error = PIDController::GetSetpoint() - processVariable;
  _integral += error * dt;
  if(dt > 0) {
    _derivative = (error - _lastError) / dt;
  } else {
    _derivative = 0;
  }
  double output = _gains.kP() * error + _gains.kI() * _integral + _gains.kD() * _derivative;
  _lastError = error;

  return output;
}