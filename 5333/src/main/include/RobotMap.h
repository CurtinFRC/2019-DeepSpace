#pragma once

#include <frc/SpeedControllerGroup.h>
#include <frc/Spark.h>

#include "CurtinCtre.h"
#include "CurtinControllers.h"
#include "Gearbox.h"
#include "actuators/DoubleSolenoid.h"
#include "sensors/Encoder.h"
#include "sensors/NavX.h"

#include "control/PIDController.h"

#include "Drivetrain.h"
#include "HarvesterIntake.h"

struct RobotMap {
  struct JoyMap {
    int holdMovement = 9; // Makes the robot 'line up' (0 magnitude but still rotating)
    int activateFOC = 10; // Toggles the drivetrain between Manual and FOC control
  };

  curtinfrc::Joystick joy{ 0 };
  JoyMap joyMap;

  struct DriveTrain {
    curtinfrc::TalonSrx leftSrx{ 1, 2048 };
    curtinfrc::VictorSpx leftSpx{ 2 };
    frc::SpeedControllerGroup leftMotors{ leftSrx, leftSpx };
    curtinfrc::Gearbox leftGearbox{ &leftMotors, &leftSrx, 10.71 };

    curtinfrc::TalonSrx rightSrx{ 3, 2048 };
    curtinfrc::VictorSpx rightSpx{ 4 };
    frc::SpeedControllerGroup rightMotors{ rightSrx, rightSpx }; 
    curtinfrc::Gearbox rightGearbox{ &rightMotors, &rightSrx, 10.71 };

    curtinfrc::sensors::NavX navx{};
    curtinfrc::sensors::NavXGyro gyro{ navx.Angular(curtinfrc::sensors::AngularAxis::YAW) };

    curtinfrc::control::PIDGains gainsFOC{ "FOC", 0.008 };
    curtinfrc::control::PIDGains gainsPOV{ "POV", 0.032 }; // temp value
  };

  DriveTrain drivetrain;

  struct Elevator {
    frc::Spark liftMotors{5};
    curtinfrc::sensors::DigitalEncoder liftEncoder{0, 1, 1024};
    curtinfrc::Gearbox elevatorGearbox{ &liftMotors, &liftEncoder, 15.79, curtinfrc::physics::DcMotor::m775pro() * 4 };

    curtinfrc::control::PIDGains lower{ "Lower Elevator", 1 };
  };

  Elevator lift;

  struct HarvesterIntake {
    frc::Spark harvesterMotor{ 6 };
    curtinfrc::actuators::DoubleSolenoid harvesterSolenoid{ 0, 1 };
    curtinfrc::Gearbox harvesterGearbox{ &harvesterMotor, nullptr, 4 };
  };

  HarvesterIntake harvesterIntake;
};
