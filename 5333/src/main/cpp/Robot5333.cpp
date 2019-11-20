#include "Robot5333.h"
#include "ControlMap.h"
#include "strategy/MPStrategy.h"

#include <math.h>
#include <iostream>

#include <cameraserver/CameraServer.h>

#include <frc/DriverStation.h>

using namespace frc;
using namespace curtinfrc;

double lastTimestamp;

void Robot::RobotInit() {
  lastTimestamp = Timer::GetFPGATimestamp();
  ControlMap::InitSmartControllerGroup(robotmap.contGroup);

  auto cameraFront = CameraServer::GetInstance()->StartAutomaticCapture(0);
  auto cameraSide = CameraServer::GetInstance()->StartAutomaticCapture(1);

  cameraFront.SetFPS(30);
  cameraSide.SetFPS(30);

  cameraFront.SetResolution(160, 120);
  cameraSide.SetResolution(160, 120);

  // Our motors are mounted backwards, but the simulation doesn't know about that.
#ifdef __FRC_ROBORIO__
  robotmap.drivetrain.rightGearbox.transmission->SetInverted(true); 
#else
  robotmap.drivetrain.leftGearbox.transmission->SetInverted(true);
#endif
  robotmap.lift.elevatorGearbox.transmission->SetInverted(true);
  robotmap.drivetrain.leftGearbox.encoder->ZeroEncoder();
  robotmap.drivetrain.rightGearbox.encoder->ZeroEncoder();

  beElevator = new Lift(robotmap.lift.config, robotmap.lift.lower);
  beElevator->SetDefault(std::make_shared<LiftManualStrategy>(*beElevator, robotmap.contGroup));
  beElevator->StartLoop(100);

  drivetrain = new Drivetrain(robotmap.drivetrain.config, robotmap.drivetrain.gainsVelocity);
  drivetrain->SetDefault(std::make_shared<DrivetrainManualStrategy>(*drivetrain, *beElevator, robotmap.contGroup));
  drivetrain->StartLoop(100);
  stratFOC = std::make_shared<DrivetrainFOCStrategy>(*drivetrain, robotmap.contGroup, robotmap.drivetrain.gainsFOC);

  // sideHatchIntake = new HatchIntake(robotmap.sideHatchIntake.config);
  // sideHatchIntake->SetDefault(std::make_shared<HatchIntakeManualStrategy>(*sideHatchIntake, robotmap.contGroup, true));
  // sideHatchIntake->StartLoop(50);

  // frontHatchIntake = new HatchIntake(robotmap.frontHatchIntake.config);
  // frontHatchIntake->SetDefault(std::make_shared<HatchIntakeManualStrategy>(*frontHatchIntake, robotmap.contGroup, false));
  // frontHatchIntake->StartLoop(50);

  boxIntake = new BoxIntake(robotmap.boxIntake.config);
  boxIntake->SetDefault(std::make_shared<BoxIntakeManualStrategy>(*boxIntake, robotmap.contGroup));
  boxIntake->StartLoop(50);

  StrategyController::Register(drivetrain);
  StrategyController::Register(beElevator);
  // StrategyController::Register(sideHatchIntake);
  // StrategyController::Register(frontHatchIntake);
  StrategyController::Register(boxIntake);


  NTProvider::Register(&robotmap.controlSystem.pressureSensor);

  NTProvider::Register(drivetrain);
  NTProvider::Register(beElevator);
  // NTProvider::Register(sideHatchIntake);
  // NTProvider::Register(frontHatchIntake);
  NTProvider::Register(boxIntake);
}

static bool goalExtend = actuators::kReverse;
static bool goalGrasp = actuators::kForward;

void Robot::RobotPeriodic() {
  double dt = Timer::GetFPGATimestamp() - lastTimestamp;
  lastTimestamp = Timer::GetFPGATimestamp();
  robotmap.contGroup.Update(); // update selectors, etc. [OPTIONAL]


  robotmap.sideHatchIntake.extendSolenoid.Update(dt);
  robotmap.sideHatchIntake.graspSolenoid.Update(dt);
  

  if (enableFOC && drivetrain->GetActiveStrategy() != stratFOC) enableFOC = false;
  if (robotmap.contGroup.Get(ControlMap::activateFOC, controllers::Controller::ONRISE)) {
    enableFOC = !enableFOC;
    if (enableFOC) Schedule(stratFOC);
    else stratFOC->SetDone();
  }

  double pitch = std::abs(robotmap.drivetrain.pitchGgyro.GetAngle()), roll = std::abs(robotmap.drivetrain.rollGyro.GetAngle());
  // std::cout << "P: " << pitch << ", R: " << roll << std::endl;

  if (pitch > 25 || roll > 20) {//(std::atan(std::sqrt(std::pow(std::tan(pitch), 2) + std::pow(std::tan(roll), 2))) > 30) { // If we're tipping...
    if (fallToggle.Update(true)) {
      Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointGround), true); // force beElevator to ground

      std::cout << "Grounded beElevator from tipping at " << DriverStation::GetInstance().GetMatchTime() << " sec(s) remaining";
    }
  } else {
    fallToggle.Update(false);

    if (robotmap.contGroup.Get(ControlMap::lowerLift, controllers::Controller::ONRISE) || robotmap.contGroup.Get(ControlMap::raiseLift, controllers::Controller::ONRISE)) {
      Schedule(std::make_shared<LiftManualStrategy>(*beElevator, robotmap.contGroup), true);
    }

    if (robotmap.contGroup.Get(ControlMap::liftGoalGround, controllers::Controller::ONRISE)) {
      Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointGround));

    } else if (robotmap.contGroup.Get(ControlMap::liftGoalLower1, controllers::Controller::ONRISE)) {
      Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointLower1));
    } else if (robotmap.contGroup.Get(ControlMap::liftGoalLower2, controllers::Controller::ONRISE)) {
      Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointLower2));

    } else if (robotmap.contGroup.Get(ControlMap::liftGoalMiddle1, controllers::Controller::ONRISE)) {
      Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointMiddle1));
    } else if (robotmap.contGroup.Get(ControlMap::liftGoalMiddle2, controllers::Controller::ONRISE)) {
      Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointMiddle2));

    } else if (robotmap.contGroup.Get(ControlMap::liftGoalUpper1, controllers::Controller::ONRISE)) {
      Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointUpper1));
    } else if (robotmap.contGroup.Get(ControlMap::liftGoalUpper2, controllers::Controller::ONRISE)) {
      Schedule(std::make_shared<LiftGotoStrategy>(*beElevator, ControlMap::liftSetpointUpper2));
    }
  }

  bool  grasp = robotmap.contGroup.Get(ControlMap::hatchGrab, controllers::Controller::ONRISE),
        release = robotmap.contGroup.Get(ControlMap::hatchRelease, controllers::Controller::ONRISE),
        withdraw= robotmap.contGroup.Get(ControlMap::hatchStow, controllers::Controller::ONRISE);


  if (grasp || release) {
    goalExtend = actuators::kForward;
    goalGrasp = grasp ? actuators::kForward : actuators::kReverse;
  } else if (withdraw) {
    goalExtend = actuators::kReverse;
  }

  if (robotmap.sideHatchIntake.graspSolenoid.IsDone() || goalExtend == actuators::kForward) {
    robotmap.sideHatchIntake.extendSolenoid.SetTarget(goalExtend ? actuators::kForward : actuators::kReverse);
  }

  if (robotmap.sideHatchIntake.extendSolenoid.IsDone()) {
    robotmap.sideHatchIntake.graspSolenoid.SetTarget(goalGrasp ? actuators::kForward : actuators::kReverse);
  }

  // Need to schedule stratPOV *

  // Redundant, as it can already be accessed on shuffleboard via nt, but ~
  // frc::SmartDashboard::PutNumber("Hatch Distance", robotmap.controlSystem.hatchDistanceEntry.GetDouble(-1));
  // frc::SmartDashboard::PutNumber("Hatch X Offset", robotmap.controlSystem.hatchXoffsetEntry.GetDouble(0));
  // frc::SmartDashboard::PutNumber("Hatch Y Offset", robotmap.controlSystem.hatchYoffsetEntry.GetDouble(0));
  
  // frc::SmartDashboard::PutNumber("Tape Distance", robotmap.controlSystem.tapeDistanceEntry.GetDouble(-1));
  // frc::SmartDashboard::PutNumber("Tape Angle", robotmap.controlSystem.tapeAngleEntry.GetDouble(0));
  // frc::SmartDashboard::PutNumber("Tape Target", robotmap.controlSystem.tapeTargetEntry.GetDouble(-1));

  frc::SmartDashboard::PutNumber("Elev Height", beElevator->GetHeight());
  

  // if (robotmap.contGroup.Get(ControlMap::compressorOn, controllers::Controller::ONRISE)) 
    robotmap.controlSystem.compressor.SetTarget(actuators::BinaryActuatorState::kForward);
  
  robotmap.controlSystem.compressor.Update(dt);

  StrategyController::Update(dt);
  NTProvider::Update();
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
