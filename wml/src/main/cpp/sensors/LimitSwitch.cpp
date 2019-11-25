#include "sensors/LimitSwitch.h"

wml::sensors::LimitSwitch::LimitSwitch(int channel, bool invert, std::string name) : BinarySensor(name), _switch(channel), _inverted(invert) { }

void wml::sensors::LimitSwitch::SetInverted(bool invert) {
  _inverted = invert;
}

bool wml::sensors::LimitSwitch::Get() {
  return _inverted ? !_switch.Get() : _switch.Get();
}
