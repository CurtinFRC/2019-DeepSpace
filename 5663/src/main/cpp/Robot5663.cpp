#include "Robot5663.h"
#include <math.h>

using hand = frc::XboxController::JoystickHand; // Type alias for hand

void Robot::RobotInit() {
  // Sparks
  leftMotors = new frc::Spark(0);
  rightMotors = new frc::Spark(1);
  shooter = new frc::Spark(2);
  intake = new frc::Spark(3);

  xbox = new frc::XboxController(0);
}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {}
void Robot::TeleopPeriodic() {
  // Tank drive and intake
  double leftSpeed = -xbox->GetY(hand::kLeftHand);
  double rightSpeed = xbox->GetY(hand::kRightHand);
  //double intakeSpeed = xbox->GetTriggerAxis(hand::kLeftHand);

  leftSpeed *= std::abs(leftSpeed);
  rightSpeed *= std::abs(rightSpeed);
  //intakeSpeed *= std::abs(intakeSpeed);

  leftMotors->Set(leftSpeed);
  rightMotors->Set(rightSpeed);
  //intake->Set(intakeSpeed);

  // Shooter
  if(xbox->GetTriggerAxis(hand::kLeftHand) != 0.0) {
    intake->Set(-xbox->GetTriggerAxis(hand::kLeftHand));
  }
  else {
    intake->Set(xbox->GetTriggerAxis(hand::kRightHand));
  }
}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}
