#include "actuators/Compressor.h"

void wml::actuators::Compressor::UpdateActuator(double dt) {
  if (_state == actuators::kForward) { // charge
    Start();
  } else { // off
    Stop();
  }
}

bool wml::actuators::Compressor::IsDone() {
  if (_state == actuators::kForward) { // charge
    return !Enabled();
  } else { // off
    return true;
  }
}
