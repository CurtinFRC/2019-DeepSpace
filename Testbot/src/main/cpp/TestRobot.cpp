#include "TestRobot.h"

#include <math.h>
#include <iostream>

#define JOY_EPS 0.05

using namespace frc;
using namespace curtinfrc;

void Robot::RobotInit() {
  joy = new curtinfrc::Joystick(0);
  
  leftMotors[0] = new Talon(1);
  leftMotors[0]->SetInverted(false);
  left = new Gearbox{new SpeedControllerGroup(*leftMotors[0]), nullptr};

  rightMotors[0] = new Talon(0);
  rightMotors[0]->SetInverted(true);
  right = new Gearbox{new SpeedControllerGroup(*rightMotors[0]), nullptr};

  hatchEjector = new DoubleSolenoid(0, 1);

  DrivetrainConfig drivetrainConfig{*left, *right};
  drivetrain = new Drivetrain(drivetrainConfig);
}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {}
void Robot::TeleopPeriodic() {
  double joyForward = 0, joyTurn = 0;

  joyForward = -joy->GetCircularisedAxisAgainst(joy->kYAxis, joy->kZAxis) * 0.9;
  if (std::abs(joyForward) < JOY_EPS) joyForward = 0;
  joyForward *= abs(joyForward);

  joyTurn = joy->GetCircularisedAxisAgainst(joy->kZAxis, joy->kYAxis) * 0.9;
  if (std::abs(joyTurn) < JOY_EPS) joyTurn = 0;
  // joyTurn *= abs(joyTurn);

  double leftSpeed = joyForward + joyTurn;
  double rightSpeed = joyForward - joyTurn;

  drivetrain->Set(leftSpeed, rightSpeed);

  drivetrain->Set(leftSpeed, rightSpeed);
}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}
