#include "sensors/LimitSwitch.h"

curtinfrc::sensors::LimitSwitch::LimitSwitch(int channel) {
  _switch = new frc::DigitalInput(channel);
}

bool curtinfrc::sensors::LimitSwitch::Get() {
  return _switch->Get();
}
