#include "Robot5333.h"

#include <math.h>
#include <iostream>

using namespace frc;
using namespace curtinfrc;

void Robot::RobotInit() {
  xbox = new XboxController(0);
  
  leftMotors[0] = new Spark(2);
  leftMotors[0]->SetInverted(false);
  left = new SpeedControllerGroup(*leftMotors[0]);

  rightMotors[0] = new Spark(3);
  rightMotors[0]->SetInverted(true);
  right = new SpeedControllerGroup(*rightMotors[0]);

  hatchEjector = new DoubleSolenoid(0, 1);
}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {}
void Robot::TeleopPeriodic() {
  double leftSpeed = -xbox->GetY(xbox->kLeftHand);
  double rightSpeed = -xbox->GetY(xbox->kRightHand);

  leftSpeed *= abs(leftSpeed);
  rightSpeed *= abs(rightSpeed);

  left->Set(leftSpeed);
  right->Set(rightSpeed);

  hatchEjector->Set(xbox->GetBumper(xbox->kRightHand) ? DoubleSolenoid::kForward : DoubleSolenoid::kReverse);
}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}
