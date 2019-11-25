#include "actuators/VoltageController.h"

#include <frc/RobotController.h>

using namespace wml;

actuators::MotorVoltageController::MotorVoltageController(frc::SpeedController *sc) : _speedController(sc) { }

void actuators::MotorVoltageController::SetVoltage(double voltage) {
  _speedController->Set(voltage / GetBusVoltage());
} 

double actuators::MotorVoltageController::GetVoltage() {
  return _speedController->Get() * GetBusVoltage();
}

void actuators::MotorVoltageController::SetInverted(bool invert) {
  _speedController->SetInverted(invert);
}

bool actuators::MotorVoltageController::GetInverted() {
  return _speedController->GetInverted();
}

double actuators::MotorVoltageController::GetBusVoltage() {
  return frc::RobotController::GetInputVoltage();
}