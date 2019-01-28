
#pragma once

#include <frc/SpeedControllerGroup.h>
#include <frc/Spark.h>

#include "CurtinCtre.h"
#include "Drivetrain.h"
#include "CurtinControllers.h"
#include "Gearbox.h"
#include "sensors/Encoder.h"

struct RobotMap {

  curtinfrc::Joystick joy{0};

  struct DriveTrain {
    curtinfrc::TalonSrx LeftSrx{1, 2048}; 
    curtinfrc::VictorSpx LeftSpx{2};
    frc::SpeedControllerGroup LeftMotors{LeftSrx, LeftSpx};
    curtinfrc::Gearbox LeftGearbox{&LeftMotors, &LeftSrx, 10.71};

    curtinfrc::TalonSrx RightSrx{3, 2048};
    curtinfrc::VictorSpx RightSpx{4};
    frc::SpeedControllerGroup RightMotors{RightSrx, RightSpx}; 
    curtinfrc::Gearbox RightGearbox{&RightMotors, &RightSrx, 10.71};
  };

  DriveTrain drivetrain; 

  struct Elevator {
    frc::Spark LiftMotors{5};
    curtinfrc::Gearbox ElevatorGearbox{&LiftMotors, nullptr, 20};
  };

  Elevator lift;
};
