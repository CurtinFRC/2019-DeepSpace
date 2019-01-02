#include "curtin_ctre.h"

using namespace curtinfrc;

void talon_srx::StopMotor() {
  Disable();
}

void talon_srx::PIDWrite(double output) {
  Set(output);
}

double talon_srx::Get() const {
  return _value;
}

void talon_srx::modify_config(std::function<void(talon_srx::configuration &)> func) {
  talon_srx::configuration config = save_config();
  func(config);
  load_config(config);
}