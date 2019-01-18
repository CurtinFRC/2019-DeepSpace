#include "Robot5333.h"

#include <math.h>
#include <iostream>

#include "cameraserver/CameraServer.h"

using namespace frc;
using namespace curtinfrc;

double lastTimestamp;

void Robot::RobotInit() {
  CameraServer::GetInstance()->StartAutomaticCapture(0);
  CameraServer::GetInstance()->StartAutomaticCapture(1);

  joy = new curtinfrc::Joystick(0);

  leftSRX = new TalonSrx(1, 2048);
  leftSRX->SetInverted(false);
  leftSPX = new VictorSpx(2);
  leftSPX->SetInverted(false);
  left = new SensoredTransmission{ new SpeedControllerGroup(*leftSRX, *leftSPX), nullptr };

  rightSRX = new TalonSrx(3, 2048);
  rightSRX->SetInverted(true);
  rightSPX = new VictorSpx(4);
  rightSPX->SetInverted(true);
  right = new SensoredTransmission{ new SpeedControllerGroup(*rightSRX, *rightSPX), nullptr };

  DrivetrainConfig drivetrainConfig{*left, *right};
  drivetrain = new Drivetrain(drivetrainConfig);


  liftMotors[0] = new Spark(5);
  liftGearbox = new Gearbox{ new SpeedControllerGroup(*liftMotors[0]), nullptr };

  ElevatorConfig elevatorConfig{ *liftGearbox, 1, nullptr, nullptr };
  beElevator = new Lift(elevatorConfig);
}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() { lastTimestamp = Timer::GetFPGATimestamp(); }
void Robot::TeleopPeriodic() {
  double dt = -lastTimestamp + (lastTimestamp = Timer::GetFPGATimestamp());
  // Calc dt for update functions
  double joyY = -joy->GetCircularisedAxisAgainst(joy->kYAxis, joy->kZAxis) * 0.9;
  double joyZ = joy->GetCircularisedAxisAgainst(joy->kZAxis, joy->kYAxis) * 0.65;

  joyY *= abs(joyY);
  joyZ *= abs(joyZ);

  double leftSpeed = joyY + joyZ;
  double rightSpeed = joyY - joyZ;

  drivetrain->Set(leftSpeed, rightSpeed);


  double beElevatorSpeed = (joy->GetRawButton(8) - joy->GetRawButton(7)) * 0.8;

  beElevator->Set(beElevatorSpeed);

  // Class update events
  beElevator->Update(dt);
}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}
