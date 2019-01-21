#include "TestRobot.h"

#include <math.h>
#include <iostream>

using namespace frc;
using namespace curtinfrc;

void Robot::RobotInit() {
  xbox = new XboxController(0);
  
  leftMotors[0] = new Spark(2);
  leftMotors[0]->SetInverted(false);
  left = new Gearbox{new SpeedControllerGroup(*leftMotors[0]), nullptr};

  rightMotors[0] = new Spark(3);
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
  double leftSpeed = -xbox->GetY(xbox->kLeftHand);
  double rightSpeed = -xbox->GetY(xbox->kRightHand);

  leftSpeed *= fabs(leftSpeed);
  rightSpeed *= fabs(rightSpeed);

  drivetrain->Set(leftSpeed, rightSpeed);

  hatchEjector->Set(!xbox->GetBumper(xbox->kRightHand) ? DoubleSolenoid::kForward : DoubleSolenoid::kReverse);
  // if (xbox->GetBumper(xbox->kRightHand)) {
  //   solState++;
  //   solState %= 2; //2;
  //   hatchEjector->Set((bool)solState ? DoubleSolenoid::kForward : DoubleSolenoid::kReverse);
  // }

  // if (xbox->GetBumper(xbox->kLeftHand)) hatchEjector->Set(DoubleSolenoid::kReverse);
}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}
