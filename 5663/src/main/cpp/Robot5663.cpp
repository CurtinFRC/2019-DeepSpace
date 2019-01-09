#include "Robot5663.h"
#include <math.h>

using hand = frc::XboxController::JoystickHand; // Type alias for hand

void Robot::RobotInit() {
  // Sparks
  left_motors = new frc::Spark(0);
  right_motors = new frc::Spark(1);
  shooter = new frc::Spark(2);
  intake = new frc::Spark(3);

  xbox = new frc::XboxController(0);
}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {}
void Robot::TeleopPeriodic() {
  // Tank drive and intake
  double left_speed = -xbox->GetY(hand::kLeftHand);
  double right_speed = xbox->GetY(hand::kRightHand);
  //double intake_speed = xbox->GetTriggerAxis(hand::kLeftHand);

  left_speed *= std::abs(left_speed);
  right_speed *= std::abs(right_speed);
  //intake_speed *= std::abs(intake_speed);

  left_motors->Set(left_speed);
  right_motors->Set(right_speed);
  //intake->Set(intake_speed);

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
