#pragma once

#include <frc/SpeedControllerGroup.h>
#include <frc/Spark.h>
#include <frc/PowerDistributionPanel.h>

#include "CurtinCtre.h"
#include "controllers/CurtinControllers.h"
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
#include "MotionProfiling.h"
// #include "strategy/MPStrategy.h"

#include "ControlMap.h"

#include "Drivetrain.h"
#include "Elevator.h"
#include "HatchIntake.h"
#include "BoxIntake.h"

struct RobotMap {
  wml::controllers::Joystick joy1{ 0 }; // Driver
  wml::controllers::Joystick joy2{ 1 }; // Co-Driver

  #if N_CONT == 2
  wml::controllers::SmartControllerGroup contGroup{ joy1, joy2 };
  #elif N_CONT == 3
  wml::controllers::XboxController xbox{ 2 };
  wml::controllers::SmartControllerGroup contGroup{ joy1, joy2, xbox };
  #endif

  // frc::PowerDistributionPanel pdp{0};

  struct DriveTrain {
    wml::TalonSrx leftSrx{ 3 };
    wml::VictorSpx leftSpx{ 4 };
    wml::actuators::MotorVoltageController leftMotors = wml::actuators::MotorVoltageController::Group(leftSrx, leftSpx);
    wml::sensors::DigitalEncoder leftEncoder{ 7, 6, 2048 };
    wml::Gearbox leftGearbox{ &leftMotors, &leftEncoder, 8.45 };

    wml::TalonSrx rightSrx{ 1 };
    wml::VictorSpx rightSpx{ 2 };
    wml::actuators::MotorVoltageController rightMotors = wml::actuators::MotorVoltageController::Group(rightSrx, rightSpx);
    wml::sensors::DigitalEncoder rightEncoder{ 4, 5, 2048 };
    wml::Gearbox rightGearbox{ &rightMotors, &rightEncoder, 8.45 };

    wml::sensors::NavX navx{frc::SPI::Port::kMXP, 200};
    wml::sensors::NavXGyro gyro{ navx.Angular(wml::sensors::AngularAxis::YAW) };
    wml::sensors::NavXGyro pitchGgyro{ navx.Angular(wml::sensors::AngularAxis::ROLL) }; // navx is 'sideways';
    wml::sensors::NavXGyro rollGyro{ navx.Angular(wml::sensors::AngularAxis::PITCH) };  // pitch <=> roll

    wml::control::PIDGains gainsFOC{ "Drivetrain FOC", 0.008, 0, 0 };
    wml::control::PIDGains gainsAlign{ "Drivetrain Align", 0.3, 0, 0.04 };
    wml::PathfinderGains gainsPathfinder{ "Drivetrain Pathfinder", 24.0, 0, 1.5, 0.36, 0.08, 12.0 / 90.0 };    // PIDVAG

    wml::DrivetrainConfig config{ leftGearbox, rightGearbox, &gyro, 0.71, 0.71, 0.0762, 50 };
    wml::control::PIDGains gainsVelocity{ "Drivetrain Velocity", 1 };
  };

  DriveTrain drivetrain;


  struct Elevator {
    wml::VictorSpx liftSpx1{ 5 };
    wml::TalonSrx liftSrx1{ 6 };
    wml::TalonSrx liftSrx2{ 7 };
    wml::VictorSpx liftSpx2{ 8 };
    wml::actuators::MotorVoltageController liftMotors = wml::actuators::MotorVoltageController::Group(liftSpx1, liftSrx1, liftSrx2, liftSpx2);

    wml::sensors::DigitalEncoder liftEncoder{ 2, 3, 2048 };
    wml::Gearbox elevatorGearbox{ &liftMotors, &liftEncoder, 15.79, wml::physics::DcMotor::m775pro() * 4 };

    wml::sensors::LimitSwitch bottomLimit{9, true};

    wml::control::PIDGains lower{ "Lower Elevator", 30.0, 0.025, 1.5 };
    // wml::control::PIDGains upper{ "Upper Elevator", 1 };


    wml::ElevatorConfig config{ elevatorGearbox, nullptr, &bottomLimit, 2.1, 30 / 1000.0, 20, "beElevator" };

    Elevator() {
      liftSpx1.SetUpdateRate(200);
      liftSrx1.SetUpdateRate(200);
      liftSrx2.SetUpdateRate(200);
      liftSpx2.SetUpdateRate(200);
    }
  };

  Elevator lift;

  struct SideHatchIntake {
    // const int forward = 115;
    // const int reverse = 5;

    // wml::actuators::BinaryServo servo{ 0, forward, reverse };
    wml::actuators::DoubleSolenoid graspSolenoid{ 2, 4, 5, 0.2 };
    wml::actuators::DoubleSolenoid extendSolenoid{ 2, 6, 7, 0.7 };

    HatchIntakeConfig config{ graspSolenoid, extendSolenoid, "Demogorgon" };
  };

  SideHatchIntake sideHatchIntake;

  // struct FrontHatchIntake {
  //   wml::actuators::DoubleSolenoid manipulatorSolenoid{ 2, 3, 2, wml::actuators::DoubleSolenoid::StandardActuationTime }; // eject
  //   wml::actuators::DoubleSolenoid solenoid{ 2, 0, 1, wml::actuators::DoubleSolenoid::StandardActuationTime }; // deploy

  //   HatchIntakeConfig config{ manipulatorSolenoid, solenoid, "Shin Destroyer" };
  // };

  // FrontHatchIntake frontHatchIntake;


  struct BoxIntake {
    wml::TalonSrx boxMotor{ 9 };
    wml::Gearbox boxIntakeGearbox{ &boxMotor, nullptr };
    wml::actuators::DoubleSolenoid solenoid{ 2, 0, 1, wml::actuators::DoubleSolenoid::StandardActuationTime };


    BoxIntakeConfig config{ boxIntakeGearbox, solenoid, true };
  };

  BoxIntake boxIntake;


  struct ControlSystem {
    wml::actuators::Compressor compressor{ 1 };

    wml::sensors::PressureSensor pressureSensor{ 0 };
    
    
    // vision
    std::shared_ptr<nt::NetworkTable> visionTable = nt::NetworkTableInstance::GetDefault().GetTable("VisionTracking");
    std::shared_ptr<nt::NetworkTable> hatchTable = visionTable->GetSubTable("HatchTracking");
    std::shared_ptr<nt::NetworkTable> tapeTable = visionTable->GetSubTable("TapeTracking");
    
    nt::NetworkTableEntry hatchDistanceEntry  = hatchTable->GetEntry("Hatch Distance"),
                          hatchXoffsetEntry   = hatchTable->GetEntry("Hatch X Offset"),
                          hatchYoffsetEntry   = hatchTable->GetEntry("Hatch Y Offset"),
                          tapeDistanceEntry   = tapeTable->GetEntry("Distance"),
                          tapeAngleEntry      = tapeTable->GetEntry("Angle"),
                          tapeTargetEntry     = tapeTable->GetEntry("Target");
  };

  ControlSystem controlSystem;
};
