#pragma once

#include <frc/SpeedController.h>
#include <sensors/Encoder.h>

/* Combines the encoder and motor parts into one data structure,
so that both Spark + encoder and Talon SRX can be treated the same. */

namespace curtinfrc {
  struct SensoredTransmission {
    frc::SpeedController &motor;
    sensors::Encoder &encoder;
  };
}