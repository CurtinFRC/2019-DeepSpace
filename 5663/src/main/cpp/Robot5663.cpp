#include "Robot5663.h"
#include <math.h>

using namespace curtinfrc;
using hand = frc::XboxController::JoystickHand; // Type alias for hand

void Robot::RobotInit() {
  // Motor_Controllers
  left_motor1 = new curtinfrc::TalonSrx(0, 0);
  right_motor1 = new curtinfrc::TalonSrx(2, 0);
  left_motor2 = new curtinfrc::TalonSrx(1, 0);
  right_motor2 = new curtinfrc::TalonSrx(3, 0);

  Cargo = new frc::Spark(2);
  Rotation = new frc::Spark(3);

  // pistons
  hatch_deploy1 = new frc::DoubleSolenoid(0, 1);
  hatch_deploy2 = new frc::DoubleSolenoid(2, 3);
  hatch_deploy3 = new frc::DoubleSolenoid(4, 5);

  
  xbox1 = new frc::XboxController(0);
  xbox2 = new frc::XboxController(1);

  table = nt::NetworkTableInstance::GetDefault().GetTable("TapeTable");
  targetAngle = table->GetEntry("Angle");
  targetDistance = table->GetEntry("Distance");
  targetOffset = table->GetEntry("Target");
}

void Robot::AutonomousInit() {
  stage = 0; //0 = searching, 1 = straight at target, 2 = correct angle
  avgDistance = 0;
  avgAngle = 0;
  avgOffset = 0;
}
void Robot::AutonomousPeriodic() {
  if (targetDistance.GetDouble > 0 && stage == 0) {
    for (int i = 0; i < 3; i++) {
      avgDistance += (i + 1) * targetDistance.GetDouble; //getting weighted averages; the later values are likely more accurate
      avgAngle += (i + 1) * targetAngle.GetDouble; //since the robot has probably stopped moving
      avgOffset += (i + 1) * targetOffset.GetDouble;
    }
    avgDistance /= 6;
    avgAngle /= 6;
    avgOffset /= 6;
    stage = 1;
  }

  if (stage == 1) {
    if (abs(avgAngle < 20)) {
      //centre target then charge straight at it lol
    } else {
      stage = 2;
    }
  }

  if (stage == 2) {
    if (avgAngle > 0) { //(if robot is positioned to the left of the target)
      //turn right (90-abs(avgAngle)+avgOffset*(32/640)) (assumes horizontal FOV of 32 degrees)
      //move forward avgDistance*sin(abs(avgAngle))
      //turn left 90 degrees
      stage = 0;
    } else {
      //turn left 90-avgOffset*(32/640) (assumes horizontal FOV of 32 degrees)
      //move forward avgDistance*sin(abs(avgAngle))
      //turn right 90 degrees
      stage = 0;
    }
  }
}

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
