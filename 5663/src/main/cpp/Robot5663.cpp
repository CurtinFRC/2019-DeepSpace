#include "Robot5663.h"
#include "Drivetrain.h"
#include <frc/PowerDistributionPanel.h>
#include <cmath>
#include "frc/AnalogInput.h"
#include "WPILib.h"
#include <iostream>

using namespace curtinfrc;
using namespace frc;
using hand = frc::XboxController::JoystickHand; // Type alias for hand

double lastTimestamp;

void Robot::RobotInit() {
  CameraServer::GetInstance()->StartAutomaticCapture(0);
  CameraServer::GetInstance()->StartAutomaticCapture(1);
  timer = new frc::Timer();
  lastTimestamp = Timer::GetFPGATimestamp();
  AI = new frc::AnalogInput(3);
  DI = new frc::DigitalInput(0);
  arduino = new I2C(frc::I2C::kOnboard, 8);
 arduino->Transaction(&message, 1, NULL, 0);
  message = 78;

  //climber
  ClimbLeft = new Spark(0);
  ClimbRight = new Spark(1);
  BIGBOYS = new DoubleSolenoid(9, 4, 5);

  //Mechanisms
  cargo = new Cargo(6,11,8);
  hatch = new Hatch(1,0,1,2,3,0);
  driveFunct = new DriveFunc(2,5,3,4);

  // Motor_Controllers
  leftTalon = new TalonSrx(2, 2048);
  leftTalon->ModifyConfig([](curtinfrc::TalonSrx::Configuration &config)  {

  });
  leftVictor = new VictorSpx(3);
  Left = new Gearbox{ new curtinfrc::actuators::MotorVoltageController(new SpeedControllerGroup(*leftTalon, *leftVictor)), nullptr };

  rightTalon = new TalonSrx(5, 2048);
  rightVictor = new VictorSpx(4);
  Right = new Gearbox{ new curtinfrc::actuators::MotorVoltageController(new SpeedControllerGroup(*rightTalon, *rightVictor)), nullptr };

  DrivetrainConfig drivetrainConfig{*Left, *Right};
  drivetrain = new Drivetrain(drivetrainConfig);
  drivetrain->StartLoop(100);
  
  xbox1 = new frc::XboxController(0);
  xbox2 = new frc::XboxController(1);

 // new PowerDistributionPanel(0);
  compressor = new Compressor(9);           //initiate compressor with PCM can ID
  compressor->SetClosedLoopControl(true);   //enable compressor
  timer->Start();
  //Servo *AntiFlooperFlooper = new Servo(1);

  //NetworkTable
  // Wait(1);
  auto inst = nt::NetworkTableInstance::GetDefault();
  // inst.StartServer();
  visionTable = inst.GetTable("VisionTracking");
  tapeTable = visionTable->GetSubTable("TapeTracking");
  targetAngle = tapeTable->GetEntry("Angle");
  targetDistance = tapeTable->GetEntry("Distance");
  targetOffset = tapeTable->GetEntry("Target");

//  AntiFlooperFlooper->Set(.5);
  //AntiFlooperFlooper->SetAngle(75);
}
void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {
  double dt = timer->Get() - lastTimer;
  lastTimer = timer->Get();
  
 //*-*-*-*-*-{ DRIVER }-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 // Tank drive 
  //Drive Functions
  // if (xbox1->GetAButton()){
  //    driveFunct->Forward(10000); // input distance in ticks
  // }
  // if (xbox1->GetBButton()){
  //    driveFunct->TurnNinety();
  //  }
  frc::SmartDashboard::PutNumber("target distance", targetDistance.GetDouble(-1.0));
  frc::SmartDashboard::PutNumber("target angle", targetAngle.GetDouble(-1.0));
  frc::SmartDashboard::PutNumber("target offset", targetOffset.GetDouble(-1.0));


  if (xbox1->GetYButton()) {
    
    if (xbox1->GetYButtonPressed()) {
      stage = 0; //0 = find target, 1 = readjust position, 2 = align on target, 3 = charge target
      snapshots = 0;
    }

    if (stage == 0 && targetDistance.GetDouble(-1.0) > 0 && snapshots < 3) {
      avgDistance += targetDistance.GetDouble(-1.0);
      avgAngle += targetAngle.GetDouble(0.0);
      avgOffset += targetOffset.GetDouble(0.0);
      snapshots += 1;
    } else if (stage == 0 && targetDistance.GetDouble(-1.0) > 0 && snapshots == 3) {
      avgAngle /= 3;
      avgDistance /= 3;
      avgOffset /= 3;

      if (abs(avgAngle) < 25) {
        stage = 2;
        snapshots = 0;
      } else {
        stage = 1;
        snapshots = 0;
      }
    }

    if (stage == 2 && abs(avgOffset) > 5) {
      double visionPower = driveFunct->TurnAngle(avgOffset * 32 / 640, dt, snapshots == 0);
      drivetrain->Set(-visionPower, -visionPower);
      snapshots++;
    } else if (abs(avgOffset) <= 5) {
      stage = 3;
      snapshots = 0;
    }

    // if (stage == 3) {
    //   driveFunct->Forward(-avgDistance, dt, snapshots == 0);
    //   snapshots++;
    // }
  } else if (xbox1->GetXButton()) {
    drivetrain->Set(0.4, -0.4);
  }
  else if (xbox1->GetBButton()) {
    drivetrain->Set(-0.4, 0.4);
  } else if (xbox1->GetBumper(hand::kLeftHand)){
    pressLBumper = xbox1->GetBumperPressed(hand::kLeftHand);
    power = driveFunct->TurnAngle(-90, dt, pressLBumper);
    drivetrain->Set(power, power);
    message = 76;
  } else if (xbox1->GetBumper(hand::kRightHand)){
    pressRBumper = xbox1->GetBumperPressed(hand::kRightHand);
    power = driveFunct->TurnAngle(90, dt, pressRBumper);
    drivetrain->Set(power, power);
    message = 76;
  } else if (xbox1->GetTriggerAxis(hand::kLeftHand)){
    double left_speed = -xbox1->GetY(hand::kLeftHand);
    double right_speed = xbox1->GetY(hand::kRightHand);
    drivetrain->Set((left_speed*std::abs(left_speed))/2, (right_speed*std::abs(right_speed))/2);
  } else {
    message = 78;
    double left_speed = -xbox1->GetY(hand::kLeftHand)/1.15;
    double right_speed = xbox1->GetY(hand::kRightHand)/1.15;
    drivetrain->Set(left_speed*std::abs(left_speed), right_speed*std::abs(right_speed));
  }

  
  // Climb
  if (xbox1->GetAButton()){
    BIGBOYS->Set(frc::DoubleSolenoid::kReverse);
  } else {
    BIGBOYS->Set(frc::DoubleSolenoid::kForward);
  }
  //CoDriver-------------------------------------------------------------------------------

  //cargo movement
  if (xbox2->GetYButton()){
     cargo->setAngle(3000);
  } else if (xbox2->GetAButton()){
     cargo->setAngle(280000);
  } else if (xbox2->GetXButton()) {
     cargo->setAngle(170000);
  } else {
     cargo->setRotationSpeed(xbox2->GetY(hand::kLeftHand));
   }
   
  //cargo intake/outtake
  if (xbox2->GetTriggerAxis(hand::kLeftHand)){
    cargo->setIntakeSpeed(-xbox2->GetTriggerAxis(hand::kLeftHand));
    // Wait(0.1);
    // cargo->setIntakeSpeed(-1);
    // Wait(0.4);
  } else {
    cargo->setIntakeSpeed(xbox2->GetTriggerAxis(hand::kRightHand)/2);
  }

  //hatch positioning
  if (xbox2->GetY(hand::kRightHand) < 0){
    if(DI->Get() == 0) {
     hatch->setRotationSpeed(xbox2->GetY(hand::kRightHand));
  
    } else {
     // hatch->alignmentPiston(false);
      hatch->setRotationSpeed(0);
    }
  } else if (xbox2->GetY(hand::kRightHand) > 0){
      if (hatch->isLocked() && DI->Get() == 1) {
        hatch->setRotationSpeed(0);
      } else {
        hatch->setRotationSpeed(xbox2->GetY(hand::kRightHand)/6);
     }
  } else {
    hatch->setRotationSpeed(0);
  //  hatch->alignmentPiston(xbox2->GetBumper(hand::kRightHand));
  }
                                    
  if (lockToggle.Update(xbox2->GetAButton())) lockState = !lockState;

  //Hatch Ejection
  hatch->ejectHatch(xbox2->GetBumper(hand::kLeftHand));
   hatch->alignmentPiston(xbox2->GetBumper(hand::kRightHand));
  hatch->lockHatch(lockState);
 

  hatch->update();
  cargo->update();
  driveFunct->update();
  frc::SmartDashboard::PutNumber("PSI", (AI->GetValue()*250/4096-25));
  frc::SmartDashboard::PutBoolean("Limit Hatch", DI->Get());
  //Update(dt);
}

void Robot::TeleopInit() {
  hatch->zeroEncoder();
  cargo->zeroEncoder();
  driveFunct->zero();
  
}

void Robot::TeleopPeriodic() {
  double dt = timer->Get() - lastTimer;
  lastTimer = timer->Get();
  
 //*-*-*-*-*-{ DRIVER }-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 // Tank drive 
  //Drive Functions
  // if (xbox1->GetAButton()){
  //    driveFunct->Forward(10000); // input distance in ticks
  // }
  // if (xbox1->GetBButton()){
  //    driveFunct->TurnNinety();
  //  }
  frc::SmartDashboard::PutNumber("target distance", targetDistance.GetDouble(-1.0));
  frc::SmartDashboard::PutNumber("target angle", targetAngle.GetDouble(-1.0));
  frc::SmartDashboard::PutNumber("target offset", targetOffset.GetDouble(-1.0));


  if (xbox1->GetYButton()) {
    
    if (xbox1->GetYButtonPressed()) {
      stage = 0; //0 = find target, 1 = readjust position, 2 = align on target, 3 = charge target
      snapshots = 0;
    }

    if (stage == 0 && targetDistance.GetDouble(-1.0) > 0 && snapshots < 3) {
      avgDistance += targetDistance.GetDouble(-1.0);
      avgAngle += targetAngle.GetDouble(0.0);
      avgOffset += targetOffset.GetDouble(0.0);
      snapshots += 1;
    } else if (stage == 0 && targetDistance.GetDouble(-1.0) > 0 && snapshots == 3) {
      avgAngle /= 3;
      avgDistance /= 3;
      avgOffset /= 3;

      if (abs(avgAngle) < 25) {
        stage = 2;
        snapshots = 0;
      } else {
        stage = 1;
        snapshots = 0;
      }
    }

    if (stage == 2 && abs(avgOffset) > 5) {
      double visionPower = driveFunct->TurnAngle(avgOffset * 32 / 640, dt, snapshots == 0);
      drivetrain->Set(-visionPower, -visionPower);
      snapshots++;
    } else if (abs(avgOffset) <= 5) {
      stage = 3;
      snapshots = 0;
    }

    // if (stage == 3) {
    //   driveFunct->Forward(-avgDistance, dt, snapshots == 0);
    //   snapshots++;
    // }
  } else if (xbox1->GetXButton()) {
    drivetrain->Set(0.4, -0.4);
  }
  else if (xbox1->GetBButton()) {
    drivetrain->Set(-0.4, 0.4);
  } else if (xbox1->GetBumper(hand::kLeftHand)){
    pressLBumper = xbox1->GetBumperPressed(hand::kLeftHand);
    power = driveFunct->TurnAngle(-90, dt, pressLBumper);
    drivetrain->Set(power, power);
    message = 76;
  } else if (xbox1->GetBumper(hand::kRightHand)){
    pressRBumper = xbox1->GetBumperPressed(hand::kRightHand);
    power = driveFunct->TurnAngle(90, dt, pressRBumper);
    drivetrain->Set(power, power);
    message = 76;
  } else if (xbox1->GetTriggerAxis(hand::kLeftHand)){
    double left_speed = -xbox1->GetY(hand::kLeftHand);
    double right_speed = xbox1->GetY(hand::kRightHand);
    left_speed = left_speed < 0.05 ? 0 : left_speed;
    right_speed = right_speed < 0.05 ? 0 : right_speed;
    drivetrain->Set((left_speed*std::abs(left_speed))/2, (right_speed*std::abs(right_speed))/2);
  } else {
    message = 78;
    double left_speed = -xbox1->GetY(hand::kLeftHand);
    double right_speed = xbox1->GetY(hand::kRightHand);
    drivetrain->Set(left_speed*std::abs(left_speed), right_speed*std::abs(right_speed));
  }

  
  // Climb
  if (xbox1->GetAButton()){
    BIGBOYS->Set(frc::DoubleSolenoid::kReverse);
  } else {
    BIGBOYS->Set(frc::DoubleSolenoid::kForward);
  }
  //CoDriver-------------------------------------------------------------------------------

  //cargo movement
  if (xbox2->GetYButton()){
     cargo->setAngle(3000);
  } else if (xbox2->GetAButton()){
     cargo->setAngle(280000);
  } else if (xbox2->GetXButton()) {
     cargo->setAngle(170000);
  } else {
     cargo->setRotationSpeed(xbox2->GetY(hand::kLeftHand));
   }
   
  //cargo intake/outtake
  if (xbox2->GetTriggerAxis(hand::kLeftHand)){
    cargo->setIntakeSpeed(-xbox2->GetTriggerAxis(hand::kLeftHand));
    // Wait(0.1);
    // cargo->setIntakeSpeed(-1);
    // Wait(0.4);
  } else {
    cargo->setIntakeSpeed(xbox2->GetTriggerAxis(hand::kRightHand)/2);
  }

  //hatch positioning
  if (xbox2->GetY(hand::kRightHand) < 0){
    if(DI->Get() == 0) {
     hatch->setRotationSpeed(xbox2->GetY(hand::kRightHand));
  
    } else {
     // hatch->alignmentPiston(false);
      hatch->setRotationSpeed(0);
    }
  } else if (xbox2->GetY(hand::kRightHand) > 0){
      if (hatch->isLocked() && DI->Get() == 1) {
        hatch->setRotationSpeed(0);
      } else {
        hatch->setRotationSpeed(xbox2->GetY(hand::kRightHand)/6);
     }
  } else {
    hatch->setRotationSpeed(0);
  //  hatch->alignmentPiston(xbox2->GetBumper(hand::kRightHand));
  }
                                    
  if (lockToggle.Update(xbox2->GetAButton())) lockState = !lockState;

  //Hatch Ejection
  hatch->ejectHatch(xbox2->GetBumper(hand::kLeftHand));
   hatch->alignmentPiston(xbox2->GetBumper(hand::kRightHand));
  hatch->lockHatch(lockState);
 

  hatch->update();
  cargo->update();
  driveFunct->update();
  frc::SmartDashboard::PutNumber("PSI", (AI->GetValue()*250/4096-25));
  frc::SmartDashboard::PutBoolean("Limit Hatch", DI->Get());
  //Update(dt);
}

void Robot::TestInit(){}
void Robot::TestPeriodic(){}