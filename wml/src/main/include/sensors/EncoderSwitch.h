#pragma once

#include <cmath>
#include <string>

#include "Encoder.h"
#include "BinarySensor.h"

namespace curtinfrc {
namespace sensors {
  class EncoderSwitch : public BinarySensor {
   public:
    EncoderSwitch(Encoder *encoder, double lowerBoundary = 0, double upperBoundary = INFINITY, std::string name = "<Encoder Switch>") : _encoder(encoder), _lower(lowerBoundary), _upper(upperBoundary) {};

    bool Get() override;

   private:
    Encoder *_encoder;
    double _lower, _upper;
  };
}  // namespace sensors
}  // namespace curtinfrc
