#include "strategy/MPStrategy.h"

#include <frc/Timer.h>
#include <frc/RobotController.h>
#include <networktables/NetworkTableInstance.h>

#include <iostream>

using namespace curtinfrc;

// CHARACTERIZATION STRATEGY //
DrivetrainCharacterizationStrategy::DrivetrainCharacterizationStrategy(Drivetrain &drivetrain) : _drivetrain(drivetrain), Strategy("MP Characterization") {
  Requires(&drivetrain);
  SetCanBeInterrupted(true);
  SetCanBeReused(true);

  _autospeedEntry = nt::NetworkTableInstance::GetDefault().GetEntry("/robot/autospeed");
  _telemetryEntry = nt::NetworkTableInstance::GetDefault().GetEntry("/robot/telemetry");
}

void DrivetrainCharacterizationStrategy::OnUpdate(double dt) {
  double now = frc::Timer::GetFPGATimestamp();

  double leftPos = _drivetrain.GetLeftDistance();
  double rightPos = _drivetrain.GetRightDistance();

  double leftRate = _drivetrain.GetConfig().leftDrive.encoder->GetEncoderAngularVelocity() * 3.14159265 * 2 * _drivetrain.GetConfig().wheelRadius;
  double rightRate = _drivetrain.GetConfig().rightDrive.encoder->GetEncoderAngularVelocity() * 3.14159265 * 2 * _drivetrain.GetConfig().wheelRadius;

  double battery = frc::RobotController::GetInputVoltage();
  double motorVolts = battery * std::abs(_lastAutospeed);

  double autospeed = _autospeedEntry.GetDouble(0);
  _lastAutospeed = autospeed;

  _drivetrain.SetVoltage(battery * autospeed, battery * autospeed);

  _telemetryArray[0] = now;
  _telemetryArray[1] = battery;
  _telemetryArray[2] = autospeed;
  _telemetryArray[3] = motorVolts;
  _telemetryArray[4] = motorVolts;
  _telemetryArray[5] = leftPos;
  _telemetryArray[6] = rightPos;
  _telemetryArray[7] = leftRate;
  _telemetryArray[8] = rightRate;

  _telemetryEntry.SetDoubleArray(_telemetryArray);
}

// PATHFINDER STRATEGY //

PathfinderMPStrategy::PathfinderMPStrategy(Drivetrain &drivetrain, PathfinderGains gains, std::string project, std::string pathname) : _drivetrain(drivetrain), _pf(gains) {
  Requires(&drivetrain);
  SetCanBeInterrupted(true);
  SetCanBeReused(false);
  _pf.Load(project, pathname);
}

void PathfinderMPStrategy::OnStart() {
  _pf.Reset();
  _pf.SetOffset(_drivetrain.GetLeftDistance(), _drivetrain.GetRightDistance());
  _drivetrain.SetExternalLoop([&](Drivetrain &drive, double) {
    if (_pf.IsDone())
      return std::pair<double, double>{0, 0};
    return _pf.Calculate(drive.GetLeftDistance(), drive.GetRightDistance(), drive.GetConfig().gyro->GetAngle());
  });
}

void PathfinderMPStrategy::OnUpdate(double dt) {
  if (_pf.IsDone())
    SetDone();
}

void PathfinderMPStrategy::OnStop() {
  _drivetrain.SetIdle();
}