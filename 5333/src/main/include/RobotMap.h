#pragma once

#include <frc/SpeedControllerGroup.h>
#include <frc/Spark.h>

#include "CurtinCtre.h"
#include "CurtinControllers.h"
#include "Gearbox.h"
#include "actuators/BinaryServo.h"
#include "actuators/DoubleSolenoid.h"
#include "sensors/Encoder.h"
#include "sensors/LimitSwitch.h"
#include "sensors/NavX.h"

#include "control/PIDController.h"
// #include "MotionProfiling.h"
// #include "strategy/MPStrategy.h"

#include "ControlMap.h"

#include "Drivetrain.h"
#include "Elevator.h"
#include "HarvesterIntake.h"
#include "HatchIntake.h"
#include "BoxIntake.h"

struct RobotMap {
  curtinfrc::Joystick joy1{ 0 }; // Driver
  curtinfrc::Joystick joy2{ 1 }; // Co-Driver
  curtinfrc::JoystickGroup joyGroup{ joy1, joy2 };


  struct DriveTrain {
    curtinfrc::TalonSrx leftSrx{ 1 };
    curtinfrc::VictorSpx leftSpx{ 2 };
    frc::SpeedControllerGroup leftMotors{ leftSrx, leftSpx };
    curtinfrc::sensors::DigitalEncoder leftEncoder{ 6, 7, 2048 };
    curtinfrc::Gearbox leftGearbox{ &leftMotors, &leftEncoder, 10.71 };

    curtinfrc::TalonSrx rightSrx{ 3 };
    curtinfrc::VictorSpx rightSpx{ 4 };
    frc::SpeedControllerGroup rightMotors{ rightSrx, rightSpx }; 
    curtinfrc::sensors::DigitalEncoder rightEncoder{ 4, 5, 2048 };
    curtinfrc::Gearbox rightGearbox{ &rightMotors, &rightEncoder, 10.71 };

    curtinfrc::sensors::NavX navx{};
    curtinfrc::sensors::NavXGyro gyro{ navx.Angular(curtinfrc::sensors::AngularAxis::YAW) };

    curtinfrc::control::PIDGains gainsFOC{ "FOC", 0.008 };
    curtinfrc::control::PIDGains gainsPOV{ "POV", 0.032 }; // temp value


    curtinfrc::DrivetrainConfig config{ leftGearbox, rightGearbox, &gyro, 0.71, 0.71, 0.0762, 50 };

    
    // std::shared_ptr<curtinfrc::PathfinderMPMode> modeLeft = std::make_shared<curtinfrc::PathfinderMPMode>(
    //   &leftSrx, mpConfig, (mpFileBase).c_str()
    // );
    // std::shared_ptr<curtinfrc::PathfinderMPMode> modeRight = std::make_shared<curtinfrc::PathfinderMPMode>(
    //   &rightSrx, mpConfig, (mpFileBase).c_str()
    // );

    const double gyro_kp = 3 / 80;

   private:
    // std::string mpFileBase = "output/test";
    // curtinfrc::MotionProfileConfig mpConfig = {
    //   6 * 3.28,                                     // wheel diameter (in)
    //   1.0 / 0.2 * 3.28, 0, 0,                       // P, I, D
    //   3.34 / 12.0 * 3.28, 0.76 / 12.0 * 3.28        // kV, kA
    // };
  };

  DriveTrain drivetrain;


  struct Elevator {
    curtinfrc::VictorSpx liftSpx1{ 5 };
    curtinfrc::TalonSrx liftSrx1{ 6 };
    curtinfrc::TalonSrx liftSrx2{ 7 };
    curtinfrc::VictorSpx liftSpx2{ 8 };
    frc::SpeedControllerGroup liftMotors{ liftSpx1, liftSrx1, liftSrx2, liftSpx2 };

    curtinfrc::sensors::DigitalEncoder liftEncoder{ 2, 3, 2048 };
    curtinfrc::Gearbox elevatorGearbox{ &liftMotors, &liftEncoder, 15.79, curtinfrc::physics::DcMotor::m775pro() * 4 };

    curtinfrc::sensors::LimitSwitch bottomLimit{9, true};

    curtinfrc::control::PIDGains lower{ "Lower Elevator", 1 };
    // curtinfrc::control::PIDGains upper{ "Upper Elevator", 1 };


    curtinfrc::ElevatorConfig config{ elevatorGearbox, nullptr, &bottomLimit, 2.1, 25 / 1000.0, 20 };
  };

  Elevator lift;


  // UNUSED
  struct HarvesterIntake {
    frc::Spark harvesterMotor{ 6 };
    curtinfrc::actuators::DoubleSolenoid harvesterSolenoid{ 1, 2, 3 };
    curtinfrc::Gearbox harvesterGearbox{ &harvesterMotor, nullptr, 4 };

    HarvesterIntakeConfig config{ harvesterGearbox, harvesterSolenoid };
  };

  HarvesterIntake harvesterIntake;


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


    BoxIntakeConfig config{ boxIntakeGearbox, solenoid };
  };

  BoxIntake boxIntake;
};
