#include "HatchIntake.h"

void HatchIntake::IntakingPeriodic() { // Intake
  _config.servo.SetAngle(_config.forward);
}

void HatchIntake::OuttakingPeriodic() { // Eject
  _config.servo.SetAngle(_config.reverse);
}
