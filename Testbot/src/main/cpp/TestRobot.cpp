#include "TestRobot.h"

#include <actuators/VoltageController.h>

#include <math.h>
#include <iostream>

#define JOY_EPS 0.05

using namespace frc;
using namespace curtinfrc;

void Robot::RobotInit() {
<<<<<<< HEAD
  joy = new curtinfrc::Joystick(0);
=======
  xbox = new curtinfrc::XboxController(0);
>>>>>>> master
  
  leftMotors[0] = new Talon(1);
  leftMotors[0]->SetInverted(false);
  left = new Gearbox{ new curtinfrc::actuators::MotorVoltageController(new SpeedControllerGroup(*leftMotors[0])), nullptr};

  rightMotors[0] = new Talon(0);
  rightMotors[0]->SetInverted(true);
  right = new Gearbox{ new curtinfrc::actuators::MotorVoltageController(new SpeedControllerGroup(*rightMotors[0])), nullptr};

  hatchEjector = new DoubleSolenoid(0, 1);

  DrivetrainConfig drivetrainConfig{*left, *right};
  drivetrain = new Drivetrain(drivetrainConfig);
}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {}
void Robot::TeleopPeriodic() {
<<<<<<< HEAD
  double joyForward = 0, joyTurn = 0;
=======
  double leftSpeed = -xbox->GetAxis(1); // L Y axis
  double rightSpeed = -xbox->GetAxis(5); // R Y axis
>>>>>>> master

  joyForward = -joy->GetCircularisedAxisAgainst(joy->kYAxis, joy->kZAxis) * 0.9;
  if (std::abs(joyForward) < JOY_EPS) joyForward = 0;
  joyForward *= abs(joyForward);

  joyTurn = joy->GetCircularisedAxisAgainst(joy->kZAxis, joy->kYAxis) * 0.9;
  if (std::abs(joyTurn) < JOY_EPS) joyTurn = 0;
  // joyTurn *= abs(joyTurn);

<<<<<<< HEAD
  double leftSpeed = joyForward + joyTurn;
  double rightSpeed = joyForward - joyTurn;
=======
  hatchEjector->Set(!xbox->GetButton(6) ? DoubleSolenoid::kForward : DoubleSolenoid::kReverse); // R bumper
  // if (xbox->GetBumper(xbox->kRightHand)) {
  //   solState++;
  //   solState %= 2; //2;
  //   hatchEjector->Set((bool)solState ? DoubleSolenoid::kForward : DoubleSolenoid::kReverse);
  // }
>>>>>>> master

  drivetrain->Set(leftSpeed, rightSpeed);

  drivetrain->Set(leftSpeed, rightSpeed);
}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}
