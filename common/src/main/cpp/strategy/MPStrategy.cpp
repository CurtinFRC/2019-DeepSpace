#include "strategy/MPStrategy.h"

#include <frc/Timer.h>
#include <frc/RobotController.h>
#include <networktables/NetworkTableInstance.h>

#include <iostream>

using namespace curtinfrc;

// CHARACTERIZATION STRATEGY //
DrivetrainCharacterizationStrategy::DrivetrainCharacterizationStrategy(Drivetrain &drivetrain) : _drivetrain(drivetrain), Strategy("MP Characterization") {
  Requires(&drivetrain);
  SetCanBeInterrupted(false);
  SetCanBeReused(false);

  _autospeedEntry = nt::NetworkTableInstance::GetDefault().GetEntry("/robot/autospeed");
  _telemetryEntry = nt::NetworkTableInstance::GetDefault().GetEntry("/robot/telemetry");
}

void DrivetrainCharacterizationStrategy::OnUpdate(double dt) {
  double now = frc::Timer::GetFPGATimestamp();

  double leftPos = _drivetrain.GetLeftDistance();
  double rightPos = _drivetrain.GetRightDistance();

  double leftRate = _drivetrain.GetConfig().leftDrive.encoder->GetEncoderAngularVelocity() * _drivetrain.GetConfig().wheelRadius;
  double rightRate = _drivetrain.GetConfig().rightDrive.encoder->GetEncoderAngularVelocity() * _drivetrain.GetConfig().wheelRadius;

  double battery = frc::RobotController::GetInputVoltage();
  double motorVolts = battery * std::abs(_lastAutospeed);

  double autospeed = _autospeedEntry.GetDouble(0);
  _lastAutospeed = autospeed;

  _drivetrain.SetVoltage(motorVolts, motorVolts);

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

// BASE STRATEGY //

// void MotionProfileStrategy::start() {
//   _notifier = new frc::Notifier([=]() {
//     if (_running) {
//       _esc->Set(_mode->calculate());
//       done = _mode->done;
//     }
//   });
//   _mode->init();
//   _running = true;
//   _notifier->StartPeriodic(_mode->ctrl_period());
// }

// void MotionProfileStrategy::tick(double time) {}

// void MotionProfileStrategy::stop() {
//   _running = false;
//   _notifier->Stop();
//   delete _notifier;
//   _esc->StopMotor();
// }

// DRIVETRAIN //

// void DrivetrainMotionProfileStrategy::OnStart() {
//   _notifier = new frc::Notifier([=]() {
//     if (_running) {
//       double l = _mode_left->calculate();
//       double r = _mode_right->calculate();

//       if (_drivetrain.GetConfig().gyro != nullptr && _mode_left->gyro_capable() && _mode_right->gyro_capable()) {
//         double gyro = fmod(-_drivetrain.GetConfig().gyro->GetAngle(), 360);
//         double heading = _mode_left->gyro_desired();

//         double angle_error = fmod(heading - gyro, 360);
//         angle_error = angle_error > 180 ? angle_error - 360 : angle_error;
//         double turn = _gyro_kP * angle_error;

//         l -= turn;
//         r += turn;
//       }

//       _drivetrain.Set(l, r);
//     }
//   });
//   _mode_left->init();
//   _mode_right->init();
//   _running = true;
//   _notifier->StartPeriodic(_mode_left->ctrl_period());
// }

// void DrivetrainMotionProfileStrategy::OnUpdate(double dt) {}

// void DrivetrainMotionProfileStrategy::OnStop() {
//   _running = false;
//   _notifier->Stop();
//   delete _notifier;
//   _drivetrain.Set(0, 0);
// }
