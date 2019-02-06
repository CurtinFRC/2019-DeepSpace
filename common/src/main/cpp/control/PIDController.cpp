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

double PIDGains::GetkP() const {
  return _kP;
}

double PIDGains::GetkI() const {
  return _kI;
}

double PIDGains::GetkD() const {
  return _kD;
}

double PIDGains::GetkF() const {
  return _kF;
}

// PIDController

PIDController::PIDController(PIDGains gains, double setpoint) : _gains(gains), _setpoint(setpoint), _lastError(0) {}

void PIDController::SetSetpoint(double setpoint) {
  Reset();
  _setpoint = setpoint;
}

double PIDController::GetSetpoint() {
  return _setpoint;
}

double PIDController::Calculate(double processVariable, double dt) {
  double error = PIDController::GetSetpoint() - processVariable;
  _integral += error * dt;
  _derivative = dt > 0 ? (error - _lastError) / dt : 0;

  double output = _gains.GetkP() * error + _gains.GetkI() * _integral + _gains.GetkD() * _derivative;
  _lastError = error;

  return output;
}

void PIDController::Reset() {
  _integral = 0;
  _derivative = 0;
  _lastError = 0;
}
