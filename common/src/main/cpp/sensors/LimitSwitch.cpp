#include "sensors/LimitSwitch.h"

curtinfrc::sensors::LimitSwitch::LimitSwitch(int channel, bool invert, std::string name) : BinarySensor(name), _switch(channel), _inverted(invert) { }

void curtinfrc::sensors::LimitSwitch::SetInverted(bool invert) {
  _inverted = invert;
}

bool curtinfrc::sensors::LimitSwitch::Get() {
  return _inverted ? !_switch.Get() : _switch.Get();
}
