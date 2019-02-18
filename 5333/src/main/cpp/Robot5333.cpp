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

  visionTable = nt::NetworkTableInstance::GetDefault().GetTable("vision");
  yOffset = visionTable->GetEntry("yOffset");
  xOffset = visionTable->GetEntry("xOffset");
  endAngle = visionTable->GetEntry("endAngle");

  // CameraServer::GetInstance()->StartAutomaticCapture(0);
  // CameraServer::GetInstance()->StartAutomaticCapture(1);

  robotmap.drivetrain.rightGearbox.transmission->SetInverted(true); 
  robotmap.lift.elevatorGearbox.transmission->SetInverted(true);
  robotmap.drivetrain.leftGearbox.encoder->ZeroEncoder();
  robotmap.drivetrain.rightGearbox.encoder->ZeroEncoder();

  drivetrain = new Drivetrain(robotmap.drivetrain.config);
  drivetrain->SetDefault(std::make_shared<DrivetrainManualStrategy>(*drivetrain, robotmap.contGroup));
  drivetrain->StartLoop(100);
  stratFOC = std::make_shared<DrivetrainFOCStrategy>(*drivetrain, robotmap.contGroup, robotmap.drivetrain.gainsFOC);

  beElevator = new Lift(robotmap.lift.config, robotmap.lift.lower);
  beElevator->SetDefault(std::make_shared<LiftManualStrategy>(*beElevator, robotmap.contGroup));
  beElevator->StartLoop(100);

  // sideHatchIntake = new HatchIntake(robotmap.sideHatchIntake.config);
  // sideHatchIntake->SetDefault(std::make_shared<HatchIntakeManualStrategy>(*sideHatchIntake, robotmap.contGroup, false));
  // sideHatchIntake->StartLoop(50);

  frontHatchIntake = new HatchIntake(robotmap.frontHatchIntake.config);
  frontHatchIntake->SetDefault(std::make_shared<HatchIntakeManualStrategy>(*frontHatchIntake, robotmap.contGroup, true));
  frontHatchIntake->StartLoop(50);

  boxIntake = new BoxIntake(robotmap.boxIntake.config);
  boxIntake->SetDefault(std::make_shared<BoxIntakeManualStrategy>(*boxIntake, robotmap.contGroup));
  boxIntake->StartLoop(50);

  Register(drivetrain);
  Register(beElevator);
  // Register(harvester);
  // Register(sideHatchIntake);
  Register(frontHatchIntake);
  Register(boxIntake);
}

void Robot::RobotPeriodic() {
  double dt = Timer::GetFPGATimestamp() - lastTimestamp;
  lastTimestamp = Timer::GetFPGATimestamp();

  if (enableFOC && drivetrain->GetActiveStrategy() != stratFOC) enableFOC = false;
  if (robotmap.contGroup.GetButtonRise(ControlMap::activateFOC)) {
    enableFOC = !enableFOC;
    if (enableFOC) Schedule(stratFOC);
    else stratFOC->SetDone();
  }

  if (robotmap.contGroup.GetButtonRise(ControlMap::liftGoalGround)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointGround));

  } else if (robotmap.contGroup.GetButtonRise(ControlMap::liftGoalLower1)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointLower1));
  } else if (robotmap.contGroup.GetButtonRise(ControlMap::liftGoalLower2)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointLower2));

  } else if (robotmap.contGroup.GetButtonRise(ControlMap::liftGoalMiddle1)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointMiddle1));
  } else if (robotmap.contGroup.GetButtonRise(ControlMap::liftGoalMiddle2)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointMiddle2));

  } else if (robotmap.contGroup.GetButtonRise(ControlMap::liftGoalUpper1)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointUpper1));
  } else if (robotmap.contGroup.GetButtonRise(ControlMap::liftGoalUpper2)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointUpper2));
  }
  // Need to schedule stratPOV *

  frc::SmartDashboard::PutNumber("PSI", robotmap.controlSystem.pressureSensor.GetPSI());
  if (robotmap.contGroup.GetButtonRise(ControlMap::compressorOn)) robotmap.controlSystem.compressor.SetTarget(actuators::BinaryActuatorState::kForward);
  
  robotmap.controlSystem.compressor.Update(dt);
  if (robotmap.controlSystem.compressor.IsDone()) {
    robotmap.controlSystem.compressor.Stop();
  }

  Update(dt);
}

void Robot::DisabledInit() {
  InterruptAll(true);
}

void Robot::AutonomousInit() {
  // Schedule(std::make_shared<PathfinderMPStrategy>(*drivetrain, robotmap.drivetrain.gainsPathfinder, "5333", "test"));
}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {}
void Robot::TeleopPeriodic() {}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}
