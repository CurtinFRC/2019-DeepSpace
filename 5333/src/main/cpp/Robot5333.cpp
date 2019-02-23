#include "Robot5333.h"
#include "ControlMap.h"
#include "strategy/MPStrategy.h"

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

  // Our motors are mounted backwards, but the simulation doesn't know about that.
#ifdef __FRC_ROBORIO__
  robotmap.drivetrain.rightGearbox.transmission->SetInverted(true); 
#else
  robotmap.drivetrain.leftGearbox.transmission->SetInverted(true);
#endif
  robotmap.lift.elevatorGearbox.transmission->SetInverted(true);
  robotmap.drivetrain.leftGearbox.encoder->ZeroEncoder();
  robotmap.drivetrain.rightGearbox.encoder->ZeroEncoder();

  drivetrain = new Drivetrain(robotmap.drivetrain.config, robotmap.drivetrain.gainsVelocity);
  drivetrain->SetDefault(std::make_shared<DrivetrainManualStrategy>(*drivetrain, robotmap.contGroup));
  drivetrain->StartLoop(100);
  stratFOC = std::make_shared<DrivetrainFOCStrategy>(*drivetrain, robotmap.contGroup, robotmap.drivetrain.gainsFOC);

  beElevator = new Lift(robotmap.lift.config, robotmap.lift.lower);
  beElevator->SetDefault(std::make_shared<LiftManualStrategy>(*beElevator, robotmap.contGroup));
  beElevator->StartLoop(100);

  sideHatchIntake = new HatchIntake(robotmap.sideHatchIntake.config);
  sideHatchIntake->SetDefault(std::make_shared<HatchIntakeManualStrategy>(*sideHatchIntake, robotmap.contGroup, false));
  sideHatchIntake->StartLoop(50);

  frontHatchIntake = new HatchIntake(robotmap.frontHatchIntake.config);
  frontHatchIntake->SetDefault(std::make_shared<HatchIntakeManualStrategy>(*frontHatchIntake, robotmap.contGroup, true));
  frontHatchIntake->StartLoop(50);

  boxIntake = new BoxIntake(robotmap.boxIntake.config);
  boxIntake->SetDefault(std::make_shared<BoxIntakeManualStrategy>(*boxIntake, robotmap.contGroup));
  boxIntake->StartLoop(50);

  Register(drivetrain);
  Register(beElevator);
  Register(sideHatchIntake);
  Register(frontHatchIntake);
  Register(boxIntake);
}

void Robot::RobotPeriodic() {
  double dt = Timer::GetFPGATimestamp() - lastTimestamp;
  lastTimestamp = Timer::GetFPGATimestamp();

  if (enableFOC && drivetrain->GetActiveStrategy() != stratFOC) enableFOC = false;
  if (robotmap.contGroup.GetInputRise(ControlMap::activateFOC)) {
    enableFOC = !enableFOC;
    if (enableFOC) Schedule(stratFOC);
    else stratFOC->SetDone();
  }

  if (robotmap.contGroup.GetInputRise(ControlMap::liftGoalGround)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointGround));

  } else if (robotmap.contGroup.GetInputRise(ControlMap::liftGoalLower1)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointLower1));
  } else if (robotmap.contGroup.GetInputRise(ControlMap::liftGoalLower2)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointLower2));

  } else if (robotmap.contGroup.GetInputRise(ControlMap::liftGoalMiddle1)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointMiddle1));
  } else if (robotmap.contGroup.GetInputRise(ControlMap::liftGoalMiddle2)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointMiddle2));

  } else if (robotmap.contGroup.GetInputRise(ControlMap::liftGoalUpper1)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointUpper1));
  } else if (robotmap.contGroup.GetInputRise(ControlMap::liftGoalUpper2)) {
    Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointUpper2));
  }
  // Need to schedule stratPOV *

  frc::SmartDashboard::PutNumber("PSI", robotmap.controlSystem.pressureSensor.GetPSI());
  if (robotmap.contGroup.GetInputRise(ControlMap::compressorOn)) robotmap.controlSystem.compressor.SetTarget(actuators::BinaryActuatorState::kForward);
  
  // Redundant, as it can already be accessed on shuffleboard via nt, but ~
  frc::SmartDashboard::PutNumber("Hatch Distance", robotmap.controlSystem.hatchDistanceEntry.GetDouble(-1));
  frc::SmartDashboard::PutNumber("Hatch X Offset", robotmap.controlSystem.hatchXoffsetEntry.GetDouble(0));
  frc::SmartDashboard::PutNumber("Hatch Y Offset", robotmap.controlSystem.hatchYoffsetEntry.GetDouble(0));
  
  frc::SmartDashboard::PutNumber("Tape Distance", robotmap.controlSystem.tapeDistanceEntry.GetDouble(-1));
  frc::SmartDashboard::PutNumber("Tape Angle", robotmap.controlSystem.tapeAngleEntry.GetDouble(0));
  frc::SmartDashboard::PutNumber("Tape Target", robotmap.controlSystem.tapeTargetEntry.GetDouble(-1));
  
  
  robotmap.controlSystem.compressor.Update(dt);
  if (robotmap.controlSystem.compressor.IsDone()) {
    robotmap.controlSystem.compressor.Stop();
  }

  Update(dt);

  std::cout << beElevator->GetHeight() << std::endl;
}

void Robot::DisabledInit() {
  InterruptAll(true);
}

void Robot::AutonomousInit() {
  // Schedule(std::make_shared<PathfinderMPStrategy>(*drivetrain, robotmap.drivetrain.gainsPathfinder, "5333", "d2_bM"));
}
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {}
void Robot::TeleopPeriodic() {
}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}
