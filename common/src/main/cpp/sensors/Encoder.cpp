#include "sensors/Encoder.h"

using namespace curtinfrc::sensors;

double Encoder::GetEncoderRotations() {
  return GetEncoderTicks() / (double)_encoderTicksPerRotation;
}

int Encoder::GetEncoderTicksPerRotation() {
  return _encoderTicksPerRotation;
}

int DigitalEncoder::GetEncoderTicks() {
  return _nativeEncoder.Get();
}

void DigitalEncoder::ResetEncoder() {
  _nativeEncoder.Reset();
}

int DigitalEncoder::GetChannelA() {
  return _channelA;
}

int DigitalEncoder::GetChannelB() {
  return _channelB;
}