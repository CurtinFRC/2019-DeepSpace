#include "Robot5663.h"
#include <math.h>
#include "Drive.h"
#include "Hatch.h"

using namespace curtinfrc;
using namespace frc;
using hand = frc::XboxController::JoystickHand; // Type alias for hand

Drive *drive;
Hatch *flooper;

void Robot::RobotInit() {
  // Motor_Controllers
    rotateM = new TalonSrx(1, 2048);
  Drive::Drive(5, 4, 2, 3);

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
   drive->TankDrive(left_speed, right_speed);
  

  //Rotation
  double posIn = xbox1->GetTriggerAxis(hand::kRightHand) * 90;
  flooper->rotate(posIn);
  
  //Hatch Ejection
  if(xbox1->GetBumper(hand::kLeftHand) == 1){
    hatch_deploy1->frc::DoubleSolenoid::Set(frc::DoubleSolenoid::kForward);
    } else  { 
    hatch_deploy1-> frc::DoubleSolenoid::Set(frc::DoubleSolenoid::kReverse);
    }


  }

void Robot::TestInit() {}
void Robot::TestPeriodic() {}
