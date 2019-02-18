#pragma once

#include "MotionProfiling.h"
#include "strategy/Strategy.h"
#include "Drivetrain.h"

#include <frc/Notifier.h>
#include <fstream>

#include <frc/interfaces/Gyro.h>

namespace curtinfrc {

  /**
   * Strategy used to work with the RobotPy data characterization program.
   * https://github.com/robotpy/robot-characterization
   * 
   * To make this work, schedule this strategy and ensure that it is not interrupted.
   * 
   * Note that this reports values in metric, whereas the robotpy program uses feet.
   */
  class DrivetrainCharacterizationStrategy : public Strategy {
   public:
    DrivetrainCharacterizationStrategy(Drivetrain &drivetrain);

    void OnUpdate(double dt) override;
   private:
    Drivetrain &_drivetrain;
    nt::NetworkTableEntry _autospeedEntry, _telemetryEntry;
    std::array<double, 9> _telemetryArray;
    double _lastAutospeed = 0;
  };

  // class MotionProfileStrategy : public Strategy {
  // public:
  //   MotionProfileStrategy(
  //     std::shared_ptr<MotionProfilingMode> mode,
  //     frc::SpeedController &esc
  //   ) : Strategy("Motor MP"), _mode(mode), _esc(esc) {};

  //   void OnStart() override;
  //   void OnUpdate(double dt) override;
  //   void OnStop() override;

  // private:
  //   std::shared_ptr<MotionProfilingMode> _mode;
  //   frc::SpeedController &_esc;
  //   bool _running;

  //   frc::Notifier *_notifier;
  // };

  // class DrivetrainMotionProfileStrategy : public Strategy {
  // public:
  //   DrivetrainMotionProfileStrategy(
  //     std::shared_ptr<MotionProfilingMode> mode_left, std::shared_ptr<MotionProfilingMode> mode_right,
  //     Drivetrain &drivetrain,
  //     // frc::Gyro *gyro,      // Set to nullptr to not use the gyroscope  // curtinfrc::Drivetrain has a gyro
  //     double gyro_kP
  //   ) : Strategy("Drivetrain MP"), _mode_left(mode_left), _mode_right(mode_right), _drivetrain(drivetrain), _gyro_kP(gyro_kP) {
  //     Requires(&drivetrain);
  //     SetCanBeInterrupted(true);
  //     SetCanBeReused(false);
  //   };

  //   void OnStart() override;
  //   void OnUpdate(double dt) override;
  //   void OnStop() override;

  // private:
  //   std::shared_ptr<MotionProfilingMode> _mode_left, _mode_right;
  //   Drivetrain &_drivetrain;
  //   double _gyro_kP;
  //   frc::Notifier *_notifier;
  //   bool _running;
  // };
} // ns curtinfrc
