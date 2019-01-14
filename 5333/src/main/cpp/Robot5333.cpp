#include "Robot5333.h"

#include <math.h>
#include <iostream>

using namespace frc;
using namespace curtinfrc;

void Robot::RobotInit() {
  joy = new Joystick(0);
  
  leftMotors[0] = new Spark(2);
  leftMotors[0]->SetInverted(false);
  left = new SpeedControllerGroup(*leftMotors[0]);

  rightMotors[0] = new Spark(3);
  rightMotors[0]->SetInverted(true);
  right = new SpeedControllerGroup(*rightMotors[0]);
}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {}
void Robot::TeleopPeriodic() {
  double leftSpeed = -joy->GetY() + joy->GetZ();
  double rightSpeed = -joy->GetY() - joy->GetZ();

  leftSpeed *= abs(leftSpeed);
  rightSpeed *= abs(rightSpeed);

  left->Set(leftSpeed);
  right->Set(rightSpeed);
}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}
