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

  robotmap.drivetrain.leftGearbox.transmission->SetInverted(true);

  DrivetrainConfig drivetrainConfig{ robotmap.drivetrain.leftGearbox, robotmap.drivetrain.rightGearbox, &robotmap.drivetrain.gyro, 0.71, 0.71, 0.0762, 50 };
  drivetrain = new Drivetrain(drivetrainConfig);
  drivetrain->SetDefault(std::make_shared<DrivetrainManualStrategy>(*drivetrain, robotmap.joy));
  stratFOC = std::make_shared<DrivetrainFieldOrientedControlStrategy>(*drivetrain, robotmap.joy, robotmap.drivetrain.gainsFOC);
  stratPOV = std::make_shared<DrivetrainPOVSnapStrategy>(*drivetrain, robotmap.joy, robotmap.drivetrain.gainsPOV);

  HarvesterIntakeConfig harvesterConfig{ robotmap.harvesterIntake.harvesterGearbox, robotmap.harvesterIntake.harvesterSolenoid };
  harvester = new HarvesterIntake(harvesterConfig);
  harvester->SetDefault(std::make_shared<HarvesterIntakeManualStrategy>(*harvester, robotmap.joy));
  harvester->StartLoop(50);

  ElevatorConfig elevatorConfig{ robotmap.lift.elevatorGearbox, nullptr, nullptr, 2.1, 25 / 1000.0, 20 };
  beElevator = new Lift(elevatorConfig, robotmap.lift.lower);
  beElevator->SetDefault(std::make_shared<LiftManualStrategy>(*beElevator, robotmap.joy));
  beElevator->StartLoop(100);

  Register(drivetrain);
  Register(harvester);
  Register(beElevator);
}

void Robot::RobotPeriodic() {
  double dt = Timer::GetFPGATimestamp() - lastTimestamp;
  lastTimestamp = Timer::GetFPGATimestamp();

  if (toggleFOC.Update(robotmap.joy.GetRawButton(robotmap.joyMap.activateFOC))) enableFOC = !enableFOC;

  if (enableFOC) Schedule(stratFOC);
  else if (robotmap.joy.GetPOV() != -1) Schedule(stratPOV);
  else Schedule(drivetrain->GetDefaultStrategy());

  Update(dt);
}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {}
void Robot::TeleopPeriodic() {}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}
