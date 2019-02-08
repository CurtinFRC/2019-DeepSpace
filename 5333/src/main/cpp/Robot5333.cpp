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

  drivetrain = new Drivetrain(robotmap.drivetrain.config);
  drivetrain->SetDefault(std::make_shared<DrivetrainManualStrategy>(*drivetrain, robotmap.joy));
  stratFOC = std::make_shared<DrivetrainFieldOrientedControlStrategy>(*drivetrain, robotmap.joy, robotmap.drivetrain.gainsFOC);
  stratPOV = std::make_shared<DrivetrainPOVSnapStrategy>(*drivetrain, robotmap.joy, robotmap.drivetrain.gainsPOV);

  beElevator = new Lift(robotmap.lift.config, robotmap.lift.lower);
  beElevator->SetDefault(std::make_shared<LiftManualStrategy>(*beElevator, robotmap.joy));
  beElevator->StartLoop(100);

  // harvester = new HarvesterIntake(harvesterConfig);
  // harvester->SetDefault(std::make_shared<HarvesterIntakeManualStrategy>(*harvester, robotmap.joy));
  // harvester->StartLoop(50);

  leftHatchIntake = new HatchIntake(robotmap.leftHatchIntake.config);
  leftHatchIntake->SetDefault(std::make_shared<HatchIntakeManualStrategy>(*leftHatchIntake, robotmap.joy, false));
  leftHatchIntake->StartLoop(50);

  rightHatchIntake = new HatchIntake(robotmap.rightHatchIntake.config);
  rightHatchIntake->SetDefault(std::make_shared<HatchIntakeManualStrategy>(*rightHatchIntake, robotmap.joy, true));
  rightHatchIntake->StartLoop(50);

  boxIntake = new BoxIntake(robotmap.boxIntake.config);
  boxIntake->SetDefault(std::make_shared<BoxIntakeManualStrategy>(*boxIntake, robotmap.joy));
  boxIntake->StartLoop(50);

  Register(drivetrain);
  Register(beElevator);
  // Register(harvester);
  Register(leftHatchIntake);
  Register(rightHatchIntake);
  Register(boxIntake);
}

void Robot::RobotPeriodic() {
  double dt = Timer::GetFPGATimestamp() - lastTimestamp;
  lastTimestamp = Timer::GetFPGATimestamp();

  if (toggleFOC.Update(robotmap.joy.GetRawButton(ControlMap::activateFOC))) {
    enableFOC = !enableFOC;
    if (enableFOC) Schedule(stratFOC);
    else Schedule(drivetrain->GetDefaultStrategy());
  }

  Update(dt);
}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {}
void Robot::TeleopPeriodic() {}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}
