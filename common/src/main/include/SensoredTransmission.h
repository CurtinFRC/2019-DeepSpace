#pragma once

#include <frc/SpeedController.h>
#include <sensors/Encoder.h>

/* Combines the encoder and motor parts into one data structure,
so that both Spark + encoder and Talon SRX can be treated the same. */

namespace curtinfrc {
  class SensoredTransmission {
   public:
    // Passing _transmission and _encoder
    SensoredTransmission(frc::SpeedController &_transmission, sensors::Encoder &_encoder) : transmission(_transmission), encoder(_encoder) {};
    frc::SpeedController &transmission;
    sensors::Encoder &encoder;
  };
}