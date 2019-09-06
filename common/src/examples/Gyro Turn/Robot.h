#pragma once

// WPIlib includes:

#include <frc/DriverStation.h>
#include <frc/Timer.h>
#include <frc/TimedRobot.h>

#include <frc/smartdashboard/SmartDashboard.h>
#include <networktables/NetworkTableInstance.h>


// CurtinFRC includes:

#include "NTProvider.h"                 // logs sensors and actuators to NetworkTables

#include "actuators/VoltageController.h"// Generalisation of a motor controlled by Voltage
#include "CurtinCtre.h"                 // Motor wrappers - inc. TalonSRX
#include "Drivetrain.h"                 // Drivetrain
#include "Gearbox.h"                    // Pairs a VoltageController and an encoder, with other gearbox-related characteristics
#include "sensors/NavX.h"               // NavX Gyro Sensor


class Robot : public frc::TimedRobot, protected curtinfrc::NTProvider {
 public:
  void RobotInit() override;
  void RobotPeriodic() override;


  // Create a new NavX plugged into the default port (SPI MXP)
  curtinfrc::sensors::NavX navx{};


  // Create all the Left Motors
  curtinfrc::TalonSrx leftTalon{ 1 };
  curtinfrc::actuators::MotorVoltageController leftMotors{ leftTalon };

  curtinfrc::Gearbox leftGearbox{ &leftMotors, nullptr };


  // Create all the Right Motors
  curtinfrc::TalonSrx rightTalon{ 2 };
  curtinfrc::actuators::MotorVoltageController rightMotors{ rightTalon };

  curtinfrc::Gearbox rightGearbox{ &rightMotors, nullptr };


  curtinfrc::Drivetrain *drivetrain;


  // Create a 'PID Controller' that will handle all of the turn's PID calculations
  curtinfrc::control::PIDController pid{
    { "Gyro Turn Gains", 0.3, 0, 0.04 },    // PID Gains - will have to be tuned to work with your specifc robot design
    90                                      // Turning 90 degrees
  };
};
