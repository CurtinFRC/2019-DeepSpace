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

double Encoder::GetEncoderAngularVelocity() {
  return GetEncoderTickVelocity() / _encoderTicksPerRotation * 2 * 3.1415926;
}

int DigitalEncoder::GetEncoderTicks() {
  return _nativeEncoder.Get();
}

double DigitalEncoder::GetEncoderTickVelocity() {
  return 1.0 / _nativeEncoder.GetPeriod();
}

int DigitalEncoder::GetChannelA() {
  return _channelA;
}

int DigitalEncoder::GetChannelB() {
  return _channelB;
}