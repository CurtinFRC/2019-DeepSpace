#include "strategy/MPStrategy.h"
#include <iostream>

using namespace curtinfrc;

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
