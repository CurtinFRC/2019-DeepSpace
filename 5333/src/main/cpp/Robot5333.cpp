#include "Robot5333.h"

#include <math.h>
#include <iostream>

#include "frc/Timer.h"

void Robot::RobotInit() {
  
}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {}
void Robot::TeleopPeriodic() {
  double time = frc::Timer::GetFPGATimestamp();
  s1.Set(std::sin(time));
  s2.Set(std::cos(time));
  s9.Set(-1.0);
}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}
