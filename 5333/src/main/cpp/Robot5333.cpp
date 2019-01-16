#include "Robot5333.h"

#include <math.h>
#include <iostream>

#include "cameraserver/CameraServer.h"

using namespace frc;
using namespace curtinfrc;

void Robot::RobotInit() {
  CameraServer::GetInstance()->StartAutomaticCapture(0);
  CameraServer::GetInstance()->StartAutomaticCapture(1);

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
  double joyY = -joy->GetCircularisedAxisAgainst(joy->kYAxis, joy->kZAxis) * 0.9;
  double joyZ = joy->GetCircularisedAxisAgainst(joy->kZAxis, joy->kYAxis) * 0.65;

  joyY *= abs(joyY);
  joyZ *= abs(joyZ);

  double leftSpeed = joyY + joyZ;
  double rightSpeed = joyY - joyZ;

  left->Set(leftSpeed);
  right->Set(rightSpeed);
}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}
