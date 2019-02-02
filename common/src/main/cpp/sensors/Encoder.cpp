#include "sensors/Encoder.h"

using namespace curtinfrc::sensors;

double Encoder::GetEncoderRotations() {
  return (GetEncoderTicks() - _offset) / (double)_encoderTicksPerRotation;
}

int Encoder::GetEncoderTicksPerRotation() {
  return _encoderTicksPerRotation;
}

void Encoder::ZeroEncoder() {
  _offset = GetEncoderTicks();
}

int DigitalEncoder::GetEncoderTicks() {
  return _nativeEncoder.Get();
}

int DigitalEncoder::GetChannelA() {
  return _channelA;
}

int DigitalEncoder::GetChannelB() {
  return _channelB;
}