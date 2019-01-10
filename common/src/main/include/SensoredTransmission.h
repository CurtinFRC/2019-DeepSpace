#pragma once

#include <frc/SpeedController.h>
#include <sensors/Encoder.h>

namespace curtinfrc {

  /**
   * Struct for motor and encoder pairs.
   * 
   * Combines the motor and encoder parts into one data structure,
   * so that both Spark + Encoder and Talon SRX are treated the same.
   */
  struct SensoredTransmission {
    frc::SpeedController &motor;
    sensors::Encoder &encoder;
  };
}