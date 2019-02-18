#pragma once

#include <frc/SpeedControllerGroup.h>
#include <frc/Spark.h>

#include "CurtinCtre.h"
#include "CurtinControllers.h"
#include "Gearbox.h"
#include "actuators/BinaryServo.h"
#include "actuators/Compressor.h"
#include "actuators/DoubleSolenoid.h"
#include "actuators/VoltageController.h"
#include "sensors/Encoder.h"
#include "sensors/LimitSwitch.h"
#include "sensors/NavX.h"
#include "sensors/PressureSensor.h"

#include "control/PIDController.h"
// #include "MotionProfiling.h"
// #include "strategy/MPStrategy.h"

#include "ControlMap.h"

#include "Drivetrain.h"
#include "Elevator.h"
#include "HatchIntake.h"
#include "BoxIntake.h"

struct RobotMap {
  curtinfrc::Joystick joy1{ 0 }; // Driver
  curtinfrc::Joystick joy2{ 1 }; // Co-Driver
  curtinfrc::JoystickGroup joyGroup{ joy1, joy2 };

  struct DriveTrain {
    curtinfrc::TalonSrx leftSrx{ 3 };
    curtinfrc::VictorSpx leftSpx{ 4 };
    curtinfrc::actuators::MotorVoltageController leftMotors = curtinfrc::actuators::MotorVoltageController::Group(leftSrx, leftSpx);
    curtinfrc::sensors::DigitalEncoder leftEncoder{ 7, 6, 2048 };
    curtinfrc::Gearbox leftGearbox{ &leftMotors, &leftEncoder, 8.45 };

    curtinfrc::TalonSrx rightSrx{ 1 };
    curtinfrc::VictorSpx rightSpx{ 2 };
    curtinfrc::actuators::MotorVoltageController rightMotors = curtinfrc::actuators::MotorVoltageController::Group(rightSrx, rightSpx);
    curtinfrc::sensors::DigitalEncoder rightEncoder{ 4, 5, 2048 };
    curtinfrc::Gearbox rightGearbox{ &rightMotors, &rightEncoder, 8.45 };

    curtinfrc::sensors::NavX navx{};
    curtinfrc::sensors::NavXGyro gyro{ navx.Angular(curtinfrc::sensors::AngularAxis::YAW) };

    curtinfrc::control::PIDGains gainsFOC{ "FOC", 0.008 };

    curtinfrc::DrivetrainConfig config{ leftGearbox, rightGearbox, &gyro, 0.71, 0.71, 0.0762, 50 };
  };

  DriveTrain drivetrain;


  struct Elevator {
    curtinfrc::VictorSpx liftSpx1{ 5 };
    curtinfrc::TalonSrx liftSrx1{ 6 };
    curtinfrc::TalonSrx liftSrx2{ 7 };
    curtinfrc::VictorSpx liftSpx2{ 8 };
    curtinfrc::actuators::MotorVoltageController liftMotors = curtinfrc::actuators::MotorVoltageController::Group(liftSpx1, liftSrx1, liftSrx2, liftSpx2);

    curtinfrc::sensors::DigitalEncoder liftEncoder{ 2, 3, 2048 };
    curtinfrc::Gearbox elevatorGearbox{ &liftMotors, &liftEncoder, 15.79, curtinfrc::physics::DcMotor::m775pro() * 4 };

    curtinfrc::sensors::LimitSwitch bottomLimit{9, true};

    curtinfrc::control::PIDGains lower{ "Lower Elevator", 1 };
    // curtinfrc::control::PIDGains upper{ "Upper Elevator", 1 };


    curtinfrc::ElevatorConfig config{ elevatorGearbox, nullptr, &bottomLimit, 2.1, 25 / 1000.0, 20 };

    Elevator() {
      liftSpx1.SetUpdateRate(200);
      liftSrx1.SetUpdateRate(200);
      liftSrx2.SetUpdateRate(200);
      liftSpx2.SetUpdateRate(200);
    }
  };

  Elevator lift;

  struct SideHatchIntake {
    const int forward = 115;
    const int reverse = 5;

    curtinfrc::actuators::BinaryServo servo{ 0, forward, reverse };
    curtinfrc::actuators::DoubleSolenoid solenoid{ 2, 7, 6 };

    HatchIntakeConfig config{ servo, solenoid };
  };

  SideHatchIntake sideHatchIntake;

  struct FrontHatchIntake {
    curtinfrc::actuators::DoubleSolenoid manipulatorSolenoid{ 2, 3, 2 }; // eject
    curtinfrc::actuators::DoubleSolenoid solenoid{ 2, 0, 1 }; // deploy

    HatchIntakeConfig config{ manipulatorSolenoid, solenoid };
  };

  FrontHatchIntake frontHatchIntake;


  struct BoxIntake {
    curtinfrc::TalonSrx boxMotor{ 9 };
    curtinfrc::Gearbox boxIntakeGearbox{ &boxMotor, nullptr };
    curtinfrc::actuators::DoubleSolenoid solenoid{ 2, 4, 5 };


    BoxIntakeConfig config{ boxIntakeGearbox, solenoid, true };
  };

  BoxIntake boxIntake;


  struct ControlSystem {
    curtinfrc::actuators::Compressor compressor{ 1 };

    curtinfrc::sensors::PressureSensor pressureSensor{ 0 };
  };

  ControlSystem controlSystem;
};
