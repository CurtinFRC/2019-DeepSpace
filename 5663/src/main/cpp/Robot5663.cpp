#include "Robot5663.h"
#include <math.h>

using namespace curtinfrc;
using hand = frc::XboxController::JoystickHand; // Type alias for hand

void Robot::RobotInit() {
  // Motor_Controllers
  left_motor1 = new curtinfrc::TalonSrx(0, 0);
  right_motor1 = new curtinfrc::TalonSrx(1, 0);
  left_motor2 = new curtinfrc::VictorSpx(2);
  right_motor2 = new curtinfrc::VictorSpx(3);

  Cargo = new frc::Spark(2);
  Rotation = new frc::Spark(3);

  // pistons
  hatch_deploy1 = new frc::DoubleSolenoid(0, 1);
  hatch_deploy2 = new frc::DoubleSolenoid(2, 3);
  hatch_deploy3 = new frc::DoubleSolenoid(4, 5);

  
  xbox1 = new frc::XboxController(0);
  xbox2 = new frc::XboxController(1);
}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {}
void Robot::TeleopPeriodic() {
  // Tank drive 
  double left_speed = -xbox1->GetY(hand::kLeftHand);
  double right_speed = xbox1->GetY(hand::kRightHand);

  left_speed *= std::abs(left_speed);
  right_speed *= std::abs(right_speed);

  left_motor1->Set(left_speed);
  left_motor2->Set(left_speed);
  right_motor1->Set(right_speed);
  right_motor2->Set(right_speed);

  //Rotation
  double Rotation_speed = xbox2->GetY(hand::kRightHand);

  Rotation_speed *= std::abs(Rotation_speed);

  Rotation->Set(Rotation_speed);

  //Cargo
  if(xbox2->GetTriggerAxis(hand::kLeftHand) != 0.0) {
    Cargo->Set(-xbox2->GetTriggerAxis(hand::kLeftHand));
  } else {
    Cargo->Set(xbox2->GetTriggerAxis(hand::kRightHand));
  }
  
  //Hatch Ejection
  if(xbox2->GetAButton() == 1){
    hatch_deploy1->frc::DoubleSolenoid::Set  (frc::DoubleSolenoid::kForward);
    hatch_deploy2->frc::DoubleSolenoid::Set  (frc::DoubleSolenoid::kForward);
    hatch_deploy3->frc::DoubleSolenoid::Set  (frc::DoubleSolenoid::kForward);
  } else  { 
    hatch_deploy1->frc::DoubleSolenoid::Set  (frc::DoubleSolenoid::kReverse);
    hatch_deploy2->frc::DoubleSolenoid::Set  (frc::DoubleSolenoid::kReverse);
    hatch_deploy3->frc::DoubleSolenoid::Set  (frc::DoubleSolenoid::kReverse);
  }


  }

void Robot::TestInit() {}
void Robot::TestPeriodic() {}
