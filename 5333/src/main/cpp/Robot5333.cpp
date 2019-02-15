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
  stratMP = std::make_shared<DrivetrainMotionProfileStrategy>(robotmap.drivetrain.modeLeft, robotmap.drivetrain.modeRight, *drivetrain, robotmap.drivetrain.gyro_kp);

  beElevator = new Lift(robotmap.lift.config, robotmap.lift.lower);
  beElevator->SetDefault(std::make_shared<LiftManualStrategy>(*beElevator, robotmap.joyGroup));
  beElevator->StartLoop(100);

  // harvester = new HarvesterIntake(harvesterConfig);
  // harvester->SetDefault(std::make_shared<HarvesterIntakeManualStrategy>(*harvester, robotmap.joyGroup));
  // harvester->StartLoop(50);

  leftHatchIntake = new HatchIntake(robotmap.leftHatchIntake.config);
  leftHatchIntake->SetDefault(std::make_shared<HatchIntakeManualStrategy>(*leftHatchIntake, robotmap.joyGroup, false));
  leftHatchIntake->StartLoop(50);

  rightHatchIntake = new HatchIntake(robotmap.rightHatchIntake.config);
  rightHatchIntake->SetDefault(std::make_shared<HatchIntakeManualStrategy>(*rightHatchIntake, robotmap.joyGroup, true));
  rightHatchIntake->StartLoop(50);

  boxIntake = new BoxIntake(robotmap.boxIntake.config);
  boxIntake->SetDefault(std::make_shared<BoxIntakeManualStrategy>(*boxIntake, robotmap.joyGroup));
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

  if (enableFOC && drivetrain->GetActiveStrategy() != stratFOC)
    enableFOC = false;
  
  if (toggleFOC.Update(robotmap.joy.GetRawButton(ControlMap::activateFOC))) {
    enableFOC = !enableFOC;
    if (enableFOC) Schedule(stratFOC);
    else stratFOC->SetDone();
  }
  
  if (robotmap.joyGroup.GetButton(ControlMap::liftGoalGround)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointGround));

  } else if (robotmap.joyGroup.GetButton(ControlMap::liftGoalLower1)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointLower1));
  } else if (robotmap.joyGroup.GetButton(ControlMap::liftGoalLower2)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointLower2));

  } else if (robotmap.joyGroup.GetButton(ControlMap::liftGoalMiddle1)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointMiddle1));
  } else if (robotmap.joyGroup.GetButton(ControlMap::liftGoalMiddle2)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointMiddle2));

  } else if (robotmap.joyGroup.GetButton(ControlMap::liftGoalUpper1)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointUpper1));
  } else if (robotmap.joyGroup.GetButton(ControlMap::liftGoalUpper2)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointUpper2));
  }
  // Need to schedule stratPOV *

  Update(dt);
}

void Robot::AutonomousInit() { Schedule(stratMP); }
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {}
void Robot::TeleopPeriodic() {}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}
