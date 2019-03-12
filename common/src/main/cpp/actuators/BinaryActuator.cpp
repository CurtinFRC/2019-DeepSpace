#include "actuators/BinaryActuator.h"

using namespace curtinfrc;
using namespace curtinfrc::actuators;


devices::RawStateDevice *BinaryActuator::MakeRawStateDevice(std::string name) {
  return new RawBinaryActuator(this, name);
}
