#pragma once

#include <frc/Timer.h>
#include <frc/TimedRobot.h>
#include <frc/SpeedControllerGroup.h>
#include <frc/Spark.h>
#include <frc/DoubleSolenoid.h>

#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableEntry.h>
#include <networktables/NetworkTableInstance.h>

#include "CurtinCtre.h"
#include "SensoredTransmission.h"
#include "Drivetrain.h"
#include "CurtinControllers.h"

#include "Lift.h"

class Robot : public frc::TimedRobot {
 public:
  void RobotInit() override;

  void AutonomousInit() override;
  void AutonomousPeriodic() override;

  void TeleopInit() override;
  void TeleopPeriodic() override;

  void TestInit() override;
  void TestPeriodic() override;

  std::shared_ptr<nt::NetworkTable> table;
  nt::NetworkTableEntry yOffset, xOffset, endAngle;


  curtinfrc::Joystick *joy;
  
  curtinfrc::TalonSrx *leftSRX, *rightSRX;
  curtinfrc::VictorSpx *leftSPX, *rightSPX;
  curtinfrc::SensoredTransmission *left, *right;
  curtinfrc::Drivetrain *drivetrain;

  frc::Spark *liftMotors[1];
  curtinfrc::Gearbox *liftGearbox;
  Lift *beElevator;
};
