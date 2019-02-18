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

  class PathfinderMPStrategy : public Strategy {
   public:
    PathfinderMPStrategy(Drivetrain &drivetrain, PathfinderGains gains, std::string project, std::string pathname);

    void OnStart() override;
    void OnUpdate(double dt) override;
    void OnStop() override;
   private:
    Drivetrain &_drivetrain;
    PathfinderController _pf;
  };
} // ns curtinfrc
