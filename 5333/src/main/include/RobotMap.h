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

#include "ControlMap.h"

#include "Drivetrain.h"
#include "Elevator.h"
#include "HarvesterIntake.h"
#include "HatchIntake.h"
#include "BoxIntake.h"

struct RobotMap {
  curtinfrc::Joystick joy{ 0 };


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


    curtinfrc::DrivetrainConfig config{ leftGearbox, rightGearbox, &gyro, 0.71, 0.71, 0.0762, 50 };
  };

  DriveTrain drivetrain;


  struct Elevator {
    frc::Spark liftMotor{ 5 };
    curtinfrc::sensors::DigitalEncoder liftEncoder{0, 1, 1024};
    curtinfrc::Gearbox elevatorGearbox{ &liftMotor, &liftEncoder, 15.79, curtinfrc::physics::DcMotor::m775pro() * 4 };

    curtinfrc::control::PIDGains lower{ "Lower Elevator", 1 };
    // curtinfrc::control::PIDGains upper{ "Upper Elevator", 1 };


    curtinfrc::ElevatorConfig config{ elevatorGearbox, nullptr, nullptr, 2.1, 25 / 1000.0, 20 };
  };

  Elevator lift;


  struct HarvesterIntake {
    frc::Spark harvesterMotor{ 6 };
    curtinfrc::actuators::DoubleSolenoid harvesterSolenoid{ 0, 1 };
    curtinfrc::Gearbox harvesterGearbox{ &harvesterMotor, nullptr, 4 };

    HarvesterIntakeConfig config{ harvesterGearbox, harvesterSolenoid };
  };

  HarvesterIntake harvesterIntake;


  struct HatchIntake {
    frc::Servo servo;
    curtinfrc::actuators::DoubleSolenoid solenoid;
    int forward = 60;
    int reverse = 0;


    HatchIntakeConfig config;
    HatchIntake(int servoPort, int solenoidPortA, int solenoidPortB) : servo(servoPort), solenoid(solenoidPortA, solenoidPortB), config(servo, solenoid, forward, reverse) {};
  };

  HatchIntake leftHatchIntake{ 7, 2, 3 };
  HatchIntake rightHatchIntake{ 8, 4, 5 };


  struct BoxIntake {
    frc::Spark boxMotor{ 9 };
    curtinfrc::Gearbox boxIntakeGearbox{ &boxMotor, nullptr };
    curtinfrc::actuators::DoubleSolenoid solenoid{ 6, 7 };


    BoxIntakeConfig config{ boxIntakeGearbox, solenoid };
  };

  BoxIntake boxIntake;
};
