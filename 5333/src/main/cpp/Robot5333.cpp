#include "Robot5333.h"
#include "ControlMap.h"

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

  frontHatchIntake = new FrontHatchIntake(robotmap.frontHatchIntake.config);
  frontHatchIntake->SetDefault(std::make_shared<FrontHatchIntakeManualStrategy>(*frontHatchIntake, robotmap.joy, false));
  frontHatchIntake->StartLoop(50);

  sideHatchIntake = new SideHatchIntake(robotmap.sideHatchIntake.config);
  sideHatchIntake->SetDefault(std::make_shared<SideHatchIntakeManualStrategy>(*sideHatchIntake, robotmap.joy, true));
  sideHatchIntake->StartLoop(50);

  boxIntake = new BoxIntake(robotmap.boxIntake.config);
  boxIntake->SetDefault(std::make_shared<BoxIntakeManualStrategy>(*boxIntake, robotmap.joy));
  boxIntake->StartLoop(50);

  Register(drivetrain);
  Register(beElevator);
  // Register(harvester);
  Register(frontHatchIntake);
  Register(sideHatchIntake);
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
  
  if (robotmap.joy.GetRawButton(ControlMap::goalGround)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, robotmap.joy, 0)); // Constants are here for now, due to undefined symbols issue in ControlMap *

  } else if (robotmap.joy.GetRawButton(ControlMap::goalLower1)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, robotmap.joy, 0.46));
  } else if (robotmap.joy.GetRawButton(ControlMap::goalLower2)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, robotmap.joy, 0.68));

  } else if (robotmap.joy.GetRawButton(ControlMap::goalMiddle1)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, robotmap.joy, 1.18));
  } else if (robotmap.joy.GetRawButton(ControlMap::goalMiddle2)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, robotmap.joy, 1.39));

  } else if (robotmap.joy.GetRawButton(ControlMap::goalUpper1)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, robotmap.joy, 1.89));
  } else if (robotmap.joy.GetRawButton(ControlMap::goalUpper2)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, robotmap.joy, 2.10));
  }
  // Need to schedule stratPOV *

  Update(dt);
}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {}
void Robot::TeleopPeriodic() {}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}
