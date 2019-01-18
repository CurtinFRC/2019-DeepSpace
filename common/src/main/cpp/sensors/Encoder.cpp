#include "sensors/Encoder.h"

double curtinfrc::sensors::Encoder::GetEncoderRotations() {
  return GetEncoderTicks() / (double)_encoderTicksPerRotation;
}

int curtinfrc::sensors::Encoder::GetEncoderTicksPerRotation() {
  return _encoderTicksPerRotation;
}

int curtinfrc::sensors::EncoderTranslator::GetEncoderTicks() {
  return _counterBase.Get();
}

void curtinfrc::sensors::EncoderTranslator::ResetEncoder() {
  _counterBase.Reset();
}
