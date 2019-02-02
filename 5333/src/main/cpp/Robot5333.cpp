#include "Robot5333.h"
#include "RobotMap.h"

#include <math.h>
#include <iostream>

#include <cameraserver/CameraServer.h>

using namespace frc;
using namespace curtinfrc;

double lastTimestamp;

void Robot::RobotInit() {
  lastTimestamp = Timer::GetFPGATimestamp();

  table = nt::NetworkTableInstance::GetDefault().GetTable("vision");
  yOffset = table->GetEntry("yOffset");
  xOffset = table->GetEntry("xOffset");
  endAngle = table->GetEntry("endAngle");

  CameraServer::GetInstance()->StartAutomaticCapture(0);
  CameraServer::GetInstance()->StartAutomaticCapture(1);

  DrivetrainConfig drivetrainConfig{ robotmap.drivetrain.leftGearbox, robotmap.drivetrain.rightGearbox, nullptr, 0.71, 0.71, 0.0762, 50 };
  drivetrain = new Drivetrain(drivetrainConfig);
  drivetrainController = new DrivetrainController(*drivetrain, robotmap.joy);

  HarvesterIntakeConfig harvesterConfig{ robotmap.harvesterIntake.harvesterGearbox, robotmap.harvesterIntake.harvesterSolenoid };
  harvester = new HarvesterIntake(harvesterConfig);
  harvester->SetDefault(std::make_shared<HarvesterIntakeManualStrategy>(*harvester, robotmap.joy));

  ElevatorConfig elevatorConfig{ robotmap.lift.elevatorGearbox, nullptr, nullptr, 2.1, 25 / 1000.0, 20 };
  beElevator = new Lift(elevatorConfig);
  beElevator->SetDefault(std::make_shared<LiftManualStrategy>(*beElevator, robotmap.joy));
}
void Robot::RobotPeriodic() {
  double dt = Timer::GetFPGATimestamp() - lastTimestamp;
  lastTimestamp = Timer::GetFPGATimestamp();
  // Calc dt for update functions

  Update(dt);
}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {}
void Robot::TeleopPeriodic() {
  drivetrainController->Update(0);

  double beElevatorSpeed = (robotmap.joy.GetRawButton(8) - robotmap.joy.GetRawButton(7)) * 0.8;

  beElevator->Set(beElevatorSpeed);

  // Class update events
  beElevator->Update(0);
}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}
