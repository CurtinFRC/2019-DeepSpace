#include "Robot5663.h"
#include "Drivetrain.h"
#include <frc/PowerDistributionPanel.h>
#include <cmath>

using namespace curtinfrc;
using namespace frc;
using hand = frc::XboxController::JoystickHand; // Type alias for hand

void Robot::RobotInit() {
  
  //Mechanisms
  cargo = new Cargo(6,7,8);
  hatch = new Hatch(1,0,1,2,3,0);
  driveFunct = new DriveFunc(2,5,3,4);

  // Motor_Controllers
  leftTalon = new TalonSrx(2, 2048);
  leftVictor = new VictorSpx(3);
  Left = new Gearbox{ new SpeedControllerGroup(*leftTalon, *leftVictor), nullptr };

  rightTalon = new TalonSrx(5, 2048);
  rightVictor = new VictorSpx(4);
  Right = new Gearbox{ new SpeedControllerGroup(*rightTalon, *rightVictor), nullptr };

  DrivetrainConfig drivetrainConfig{*Left, *Right};
  drivetrain = new Drivetrain(drivetrainConfig);
  
  xbox1 = new frc::XboxController(0);
  xbox2 = new frc::XboxController(1);

 // new PowerDistributionPanel(0);
  compressor = new Compressor(9);           //initiate compressor with PCM can ID
  compressor->SetClosedLoopControl(true);   //enable compressor
  //Servo *AntiFlooperFlooper = new Servo(1);

//  AntiFlooperFlooper->Set(.5);
  //AntiFlooperFlooper->SetAngle(75);
}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {
  hatch->zeroEncoder();
  cargo->zeroEncoder();
  driveFunct->zeroEncoder();
  
}

void Robot::TeleopPeriodic() {
  // DRIVER ----------------------------------------------------------------------------------------
 // Tank drive 
  double left_speed = -xbox1->GetY(hand::kLeftHand);
  double right_speed = xbox1->GetY(hand::kRightHand);
  drivetrain->Set(left_speed*std::abs(left_speed), right_speed*std::abs(right_speed));
  
  //Drive Functions
  // if (xbox1->GetAButton()){
  //   driveFunct->Forward(10000);
  // }
  // if (xbox1->GetBButton()){
  //   driveFunct->TurnNinety();
  // }
  // REEEEEEEE

  // CO-DRIVER -------------------------------------------------------------------------------------
  //cargo speed
  if (xbox2->GetBButton()){
    cargo->setRotationSpeed(xbox2->GetY(hand::kLeftHand)/2);
  }else{
    cargo->setRotationSpeed(0);
  }
  
  //cargo positioning
   if (xbox2->GetYButton()){
     cargo->setAngle(0.3, 90);
   } else {
     cargo->setAngle(0.3, 0);
   }

  //cargo intake/outtake
  if (xbox2->GetTriggerAxis(hand::kLeftHand)){
    cargo->setIntakeSpeed(-xbox2->GetTriggerAxis(hand::kLeftHand));
  } else {
    cargo->setIntakeSpeed(xbox2->GetTriggerAxis(hand::kRightHand)/2);
  }
  
  //manual hatch
  if (xbox2->GetYButton()){
    hatch->setRotationSpeed(-0.03);
  } else {
    hatch->setRotationSpeed(0);
  }

  //hatch positioning
  if (xbox2->GetYButton()){
    hatch->downPosition();
  } else if(xbox2->GetXButton()){
      hatch->upPosition();
  } else {
    hatch->setRotationSpeed(0);
  }

  if (lockToggle.tick(xbox2->GetAButton())) lockState = !lockState;

  //Hatch Ejection
  hatch->ejectHatch(xbox1->GetBumper(hand::kLeftHand));
  hatch->lockHatch(lockState);
  hatch->alignmentPiston(xbox1->GetBumper(hand::kRightHand));

  hatch->update();
  cargo->update();
  driveFunct->update();
}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}
