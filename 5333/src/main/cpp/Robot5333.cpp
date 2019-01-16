#include "Robot5333.h"

#include <math.h>
#include <iostream>

using namespace frc;
using namespace curtinfrc;

void Robot::RobotInit() {
  joy = new curtinfrc::Joystick(0);
  
  leftSRX = new TalonSrx(1);
  leftSRX->SetInverted(false);
  leftSPX = new VictorSpx(2);
  leftSPX->SetInverted(false);
  left = new SpeedControllerGroup(*leftSRX, *leftSPX);

  rightSRX = new TalonSrx(3);
  rightSRX->SetInverted(true);
  rightSPX = new VictorSpx(4);
  rightSPX->SetInverted(true);
  right = new SpeedControllerGroup(*rightSRX, *rightSPX);
}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {}
void Robot::TeleopPeriodic() {
  double joyY = -joy->GetCircularisedAxisAgainst(joy->kYAxis, joy->kZAxis) * 0.6;
  double joyZ = joy->GetCircularisedAxisAgainst(joy->kZAxis, joy->kYAxis) * 0.4;

  double leftSpeed = joyY + joyZ;
  double rightSpeed = joyY - joyZ;

  leftSpeed *= abs(leftSpeed);
  rightSpeed *= abs(rightSpeed);

  left->Set(leftSpeed);
  right->Set(rightSpeed);
}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}
