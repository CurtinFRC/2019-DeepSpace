#include "sensors/EncoderSwitch.h"

using namespace curtinfrc::sensors;

bool EncoderSwitch::Get() {
  return _lower < _encoder->GetEncoderRotations() && _encoder->GetEncoderRotations() < _upper;
}
