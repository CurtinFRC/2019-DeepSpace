#pragma once

#include <frc/SpeedController.h>

#include "Gearbox.h"

namespace curtinfrc {
  struct DrivetrainConfig {
    Gearbox &leftDrive;
    Gearbox &rightDrive;
  };
  
  class Drivetrain {
   public:
    Drivetrain(DrivetrainConfig config) : _config(config) {};

    void Set(double leftPower, double rightPower);
    void SetLeft(double leftPower);
    void SetRight(double rightPower);

   private:
    DrivetrainConfig _config;
  };
} // ns curtinfrc
