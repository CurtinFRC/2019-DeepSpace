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

  // CameraServer::GetInstance()->StartAutomaticCapture(0);
  // CameraServer::GetInstance()->StartAutomaticCapture(1);

  robotmap.drivetrain.rightGearbox.transmission->SetInverted(true); 
  robotmap.lift.elevatorGearbox.transmission->SetInverted(true);

  drivetrain = new Drivetrain(robotmap.drivetrain.config);
  drivetrain->SetDefault(std::make_shared<DrivetrainManualStrategy>(*drivetrain, robotmap.joyGroup));
  stratFOC = std::make_shared<DrivetrainFOCStrategy>(*drivetrain, robotmap.joyGroup, robotmap.drivetrain.gainsFOC);

  beElevator = new Lift(robotmap.lift.config, robotmap.lift.lower);
  beElevator->SetDefault(std::make_shared<LiftManualStrategy>(*beElevator, robotmap.joyGroup));
  beElevator->StartLoop(100);

  // sideHatchIntake = new HatchIntake(robotmap.sideHatchIntake.config);
  // sideHatchIntake->SetDefault(std::make_shared<HatchIntakeManualStrategy>(*sideHatchIntake, robotmap.joyGroup, false));
  // sideHatchIntake->StartLoop(50);

  frontHatchIntake = new HatchIntake(robotmap.frontHatchIntake.config);
  frontHatchIntake->SetDefault(std::make_shared<HatchIntakeManualStrategy>(*frontHatchIntake, robotmap.joyGroup, true));
  frontHatchIntake->StartLoop(50);

  boxIntake = new BoxIntake(robotmap.boxIntake.config);
  boxIntake->SetDefault(std::make_shared<BoxIntakeManualStrategy>(*boxIntake, robotmap.joyGroup));
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
  if (robotmap.joyGroup.GetButtonRise(ControlMap::activateFOC)) {
    enableFOC = !enableFOC;
    if (enableFOC) Schedule(stratFOC);
    else stratFOC->SetDone();
  }

  if (robotmap.joyGroup.GetButtonRise(ControlMap::liftGoalGround)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointGround));

  } else if (robotmap.joyGroup.GetButtonRise(ControlMap::liftGoalLower1)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointLower1));
  } else if (robotmap.joyGroup.GetButtonRise(ControlMap::liftGoalLower2)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointLower2));

  } else if (robotmap.joyGroup.GetButtonRise(ControlMap::liftGoalMiddle1)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointMiddle1));
  } else if (robotmap.joyGroup.GetButtonRise(ControlMap::liftGoalMiddle2)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointMiddle2));

  } else if (robotmap.joyGroup.GetButtonRise(ControlMap::liftGoalUpper1)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointUpper1));
  } else if (robotmap.joyGroup.GetButtonRise(ControlMap::liftGoalUpper2)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointUpper2));
  }
  // Need to schedule stratPOV *

  frc::SmartDashboard::PutNumber("PSI", robotmap.controlSystem.pressureSensor.GetPSI());
  if (robotmap.joyGroup.GetButtonRise(ControlMap::compressorOn)) robotmap.controlSystem.compressor.SetTarget(actuators::BinaryActuatorState::kForward);
  
  // Redundant, as it can already be accessed on shuffleboard via nt, but ~
  frc::SmartDashboard::PutNumber("Hatch Distance", hatchDistanceEntry.GetDouble(-1));
  frc::SmartDashboard::PutNumber("Hatch X Offset", hatchXoffsetEntry.GetDouble(0));
  frc::SmartDashboard::PutNumber("Hatch Y Offset", hatchYoffsetEntry.GetDouble(0));
  
  frc::SmartDashboard::PutNumber("Tape Distance", tapeDistanceEntry.GetDouble(-1));
  frc::SmartDashboard::PutNumber("Tape Angle", tapeAngleEntry.GetDouble(0));
  frc::SmartDashboard::PutNumber("Tape Target", tapeTargeteEntry.GetDouble(-1));
  
  
  robotmap.controlSystem.compressor.Update(dt);
  if (robotmap.controlSystem.compressor.IsDone()) {
    robotmap.controlSystem.compressor.Stop();
  }

  Update(dt);
}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {}
void Robot::TeleopPeriodic() {}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}
