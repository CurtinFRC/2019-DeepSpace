#include "Robot5333.h"
#include "RobotMap.h"

#include <math.h>
#include <iostream>

#include <cameraserver/CameraServer.h>

using namespace frc;
using namespace curtinfrc;

double lastTimestamp;

void Robot::RobotInit() {
  table = nt::NetworkTableInstance::GetDefault().GetTable("vision");
  yOffset = table->GetEntry("yOffset");
  xOffset = table->GetEntry("xOffset");
  endAngle = table->GetEntry("endAngle");

  CameraServer::GetInstance()->StartAutomaticCapture(0);
  CameraServer::GetInstance()->StartAutomaticCapture(1);

  DrivetrainConfig drivetrainConfig{ robotmap.drivetrain.leftGearbox, robotmap.drivetrain.rightGearbox, 0.71, 0.71, 0.0762, 50 };
  drivetrain = new Drivetrain(drivetrainConfig);

  HarvesterIntakeConfig harvesterConfig{ robotmap.harvesterIntake.harvesterGearbox, robotmap.harvesterIntake.harvesterSolenoid };
  harvester = new HarvesterIntake(harvesterConfig);

  ElevatorConfig elevatorConfig{ robotmap.lift.elevatorGearbox, nullptr, nullptr, 2.1, 25 / 1000.0, 20 };
  beElevator = new Lift(elevatorConfig, *harvester);
}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() { lastTimestamp = Timer::GetFPGATimestamp(); }
void Robot::TeleopPeriodic() {
  double dt = Timer::GetFPGATimestamp() - lastTimestamp;
  lastTimestamp = Timer::GetFPGATimestamp();
  // Calc dt for update functions
  
  double joyY = -robotmap.joy.GetCircularisedAxisAgainst(robotmap.joy.kYAxis, robotmap.joy.kZAxis) * 0.9;
  double joyZ = robotmap.joy.GetCircularisedAxisAgainst(robotmap.joy.kZAxis, robotmap.joy.kYAxis) * 0.65;

  joyY *= abs(joyY);
  joyZ *= abs(joyZ);

  double leftSpeed = joyY + joyZ;
  double rightSpeed = joyY - joyZ;

  drivetrain->Set(leftSpeed, rightSpeed);


  double beElevatorSpeed = (robotmap.joy.GetRawButton(8) - robotmap.joy.GetRawButton(7)) * 0.8;

  beElevator->Set(beElevatorSpeed);

  // Class update events
  beElevator->Update(dt);
}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}
