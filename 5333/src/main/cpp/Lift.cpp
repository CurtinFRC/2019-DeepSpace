#include "Lift.h"

Lift::Lift(curtinfrc::ElevatorConfig elevatorConfig) {
  _elevator = new curtinfrc::Elevator(elevatorConfig);
}

void Lift::Set(double power) {
  _elevator->SetManual(power);
}
