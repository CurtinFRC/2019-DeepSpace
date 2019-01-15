#pragma once

#include "SensoredTransmission.h"

namespace curtinfrc {
  class Gearbox : public SensoredTransmission {
   public:
    Gearbox(frc::SpeedController *transmission_in, sensors::Encoder *encoder_in) {
      transmission = transmission_in;
      encoder = encoder_in;
    };
  };
} // ns curtinfrc
