#include "Robot5333.h"

#include <math.h>
#include <iostream>

using namespace frc;
using namespace curtinfrc;

void Robot::RobotInit() {
  Xbox = new XboxController(0);
  Xbox2 = new XboxController(1);
  
  LeftMotors[0] = new talon_srx(0);
  LeftMotors[0]->SetInverted(false);
  LeftMotors[1] = new talon_srx(1);
  LeftMotors[1]->SetInverted(false);
  LeftMotors[1]->Set(talon_srx::control_mode::Follower, LeftMotors[0]->get_port());

  RightMotors[0] = new talon_srx(2);
  RightMotors[0]->SetInverted(true);
  RightMotors[1] = new talon_srx(3);
  RightMotors[1]->SetInverted(true);
  RightMotors[1]->Set(talon_srx::control_mode::Follower, RightMotors[0]->get_port());
};

void Robot::AutonomousInit() {};
void Robot::AutonomousPeriodic() {};

void Robot::TeleopInit() {};
void Robot::TeleopPeriodic() {
  double leftSpeed = -Xbox->GetY(Xbox->kLeftHand);
  double rightSpeed = -Xbox->GetY(Xbox->kRightHand);

  leftSpeed *= abs(leftSpeed);
  rightSpeed *= abs(rightSpeed);

  LeftMotors[0]->Set(leftSpeed);
  RightMotors[0]->Set(rightSpeed);
};

void Robot::TestInit() {};
void Robot::TestPeriodic() {};