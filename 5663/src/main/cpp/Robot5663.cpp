#include "Robot5663.h"
#include "Drivetrain.h"
#include <cmath>

using namespace curtinfrc;
using namespace frc;
using hand = frc::XboxController::JoystickHand; // Type alias for hand

void Robot::RobotInit() {
  //Mechanisms
  cargo = new Cargo(6,7,8);
  hatch = new Hatch(1,0,1,2,3);

  // Motor_Controllers
  leftTalon = new TalonSrx(2, 2048);
  leftVictor = new VictorSpx(3);
  Left = new SensoredTransmission{ new SpeedControllerGroup(*leftTalon, *leftVictor), nullptr };

  rightTalon = new TalonSrx(5, 2048);
  rightVictor = new VictorSpx(4);
  Right = new SensoredTransmission{ new SpeedControllerGroup(*rightTalon, *rightVictor), nullptr };

  DrivetrainConfig drivetrainConfig{*Left, *Right};
  drivetrain = new Drivetrain(drivetrainConfig);
  
  xbox1 = new frc::XboxController(0);
  xbox2 = new frc::XboxController(1);

  compressor = new Compressor(9);           //initiate compressor with PCM can ID
  compressor->SetClosedLoopControl(true);   //enable compressor

}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {
  hatch->zeroEncoder();
  cargo->zeroEncoder();
}

void Robot::TeleopPeriodic() {

  // Tank drive 
   double left_speed = -xbox1->GetY(hand::kLeftHand);
   double right_speed = xbox1->GetY(hand::kRightHand);
   drivetrain->Set(left_speed*std::abs(left_speed), right_speed*std::abs(right_speed));
  
  //cargo speed
  if (xbox1->GetTriggerAxis(hand::kRightHand)){
    cargo->setRotationSpeed(xbox1->GetTriggerAxis(hand::kRightHand));
  }else{
    cargo->setRotationSpeed(-xbox1->GetTriggerAxis(hand::kLeftHand));
  }
  
  //cargo positioning
  if (xbox1->GetYButton()){
    cargo->setAngle(0.1, 10);
  }

  //cargo intake/outtake
  if (xbox1->GetXButton()){
    cargo->setIntakeSpeed(0.2);
  }
  if (xbox1->GetBButton()){
    cargo->setIntakeSpeed(-0.2);
  }

  //hatch positioning
  if (xbox1->GetBumper(hand::kLeftHand)){
    hatch->downPosition();
  } else {
    hatch->upPosition();
  }

  //Hatch Ejection
  hatch->ejectHatch(xbox1->GetBumper(hand::kLeftHand));
  hatch->alignmentPiston(xbox1->GetAButton());

  hatch->update();
  cargo->update();
}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}
