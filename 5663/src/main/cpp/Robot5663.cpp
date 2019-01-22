#include "Robot5663.h"
#include <math.h>

using namespace curtinfrc;
using namespace frc;
using hand = frc::XboxController::JoystickHand; // Type alias for hand

void Robot::RobotInit() {
  // Motor_Controllers
    leftMotor1 = new TalonSrx(5, 2048);
    leftMotor2 = new VictorSpx(4);    
    rightMotor1 = new TalonSrx(2, 2048);
    rightMotor2 = new VictorSpx(3);
    rotateM = new TalonSrx(1, 2048);


  // pistons
  hatch_deploy1 = new frc::DoubleSolenoid(0, 1);

  
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

  leftMotor1->Set(left_speed);
  leftMotor2->Set(left_speed);
  rightMotor1->Set(right_speed);
  rightMotor2->Set(right_speed);

  //Rotation
  double Rotation_speed = xbox2->GetY(hand::kRightHand);

  Rotation_speed *= std::abs(Rotation_speed);

  rotateM->Set(Rotation_speed/2);

  
  //Hatch Ejection
  if(xbox2->GetAButtonPressed() == 1){
    hatch_deploy1->frc::DoubleSolenoid::Set(frc::DoubleSolenoid::kForward);
    } else  { 
    hatch_deploy1-> frc::DoubleSolenoid::Set(frc::DoubleSolenoid::kReverse);
    }


  }

void Robot::TestInit() {}
void Robot::TestPeriodic() {}
