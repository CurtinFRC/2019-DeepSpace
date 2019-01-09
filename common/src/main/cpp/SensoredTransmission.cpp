#include "SensoredTransmission.h"

// Get
double curtinfrc::SensoredTransmission::Get() {
  return encoder.getEncoderTicks(); 
}

// Set
void curtinfrc::SensoredTransmission::Set(double speed) {
  transmission.Set(speed);
}
